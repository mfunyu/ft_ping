#include "ping_struct.h"
#include "ping_icmp.h"
#include "ping_error.h"
#include "ping_print.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>

static void	_store_stats(t_stat *stats, double triptime)
{
	stats->sum += triptime;
	stats->sum_sq += calc_square(triptime);
	if (stats->min == 0 || triptime < stats->min)
		stats->min = triptime;
	if (triptime > stats->max)
		stats->max = triptime;
}

static double	_calc_triptime(t_packet *packet, struct timeval tv_reply)
{
	uint8_t			data[sizeof(struct timeval)];
	double			triptime;
	struct timeval	*tv_request;

	// avoid alignment issues
	memcpy(data, packet->icmpdata, sizeof(struct timeval));
	tv_request = (struct timeval *)data;
	triptime = diff_time(*tv_request, tv_reply);
	return (triptime);
}

static void	_set_echo_data(t_echo_data *echo_data, t_packet *packet, ssize_t ret)
{
	struct timeval	tv_recv;

	tv_recv = get_current_time();
	echo_data->type = packet->icmphdr.type;
	echo_data->code = packet->icmphdr.code;
	echo_data->icmplen = ret - sizeof(struct iphdr);
	set_ip_by_in_addr(echo_data->ip, packet->iphdr.saddr);
	if (echo_data->type == ICMP_ECHOREPLY)
	{
		echo_data->echo_sequence = ntohs(packet->icmphdr.echo_sequence);
		echo_data->echo_ttl = packet->iphdr.ttl;
		if (echo_data->icmplen >= sizeof(struct icmphdr) + sizeof(struct timeval))
			echo_data->echo_triptime = _calc_triptime(packet, tv_recv);
		else
			echo_data->echo_triptime = 0;
	}
	else
	{
		echo_data->un.error.iphdr = packet->req_iphdr;
		echo_data->un.error.icmphdr = packet->req_icmphdr;
	}
}

static bool	_is_reply(t_packet *packet, t_ping *ping)
{
	struct sockaddr_in	*addr;

	switch (packet->icmphdr.type)
	{
	/* ignore requests */
	case ICMP_ECHO:
		return (false);

	/* ignore responses to other ping */
	case ICMP_ECHOREPLY:
		if (packet->icmphdr.echo_id != htons(ping->ident))
			return (false);
		break;

	default:
		if (packet->req_icmphdr.echo_id != htons(ping->ident))
			return (false);
		addr = (struct sockaddr_in *)&ping->dst_addr;
		if (packet->req_iphdr.daddr != addr->sin_addr.s_addr)
			return (false);
	}
	return (true);
}

static ssize_t	_recv_reply(int sfd, t_packet *packet)
{
	struct iovec	iov = {
		.iov_base		= (void *)packet,
		.iov_len		= sizeof(t_packet),
	};
	struct msghdr	msg = {
		.msg_name		= NULL,
		.msg_namelen	= 0,
		.msg_iov		= &iov,
		.msg_iovlen		= 1,
	};
	ssize_t			ret;

	ret = recvmsg(sfd, &msg, MSG_DONTWAIT);
	if (ret < 0)
	{
		if (errno != EAGAIN && errno != EWOULDBLOCK)
			error_exit_strerr("recvmsg error");
	}
	return (ret);
}

static bool	_is_seq_duplicated(int sequence, t_ping *ping)
{
	int	index;
	int	shift;

	if ((size_t)sequence > ping->num_xmit)
		return (false);
	index = sequence % 64;
	shift = sequence / 64;
	/* dup if flag is already set */
	return (ping->seq_table[index] & (1 << shift));
}

static void	_set_seq_table(int sequence, t_ping *ping)
{
	int	index;
	int	shift;

	/* ignore unsent sequence */
	if ((size_t)sequence > ping->num_xmit)
		return ;
	index = sequence % 64;
	shift = sequence / 64;
	/* set flag */
	ping->seq_table[index] |= (1 << shift);
}

void	ping_recv(t_ping *ping)
{
	t_packet	packet = {0};
	ssize_t		ret;
	t_echo_data	echo_data;
	bool		is_dup;

	ret = _recv_reply(ping->sfd, &packet);
	if (ret < 0)
		return ;
	if (!_is_reply(&packet, ping))
		return ;

	_set_echo_data(&echo_data, &packet, ret);
	is_dup = false;
	ping->num_all++;
	if (echo_data.type == ICMP_ECHOREPLY)
	{
		if (!icmp_is_correct_checksum(&packet.icmphdr, echo_data.icmplen))
			fprintf(stderr, "checksum mismatch from %s\n", echo_data.ip);
		if (_is_seq_duplicated(echo_data.echo_sequence, ping))
		{
			ping->num_dup++;
			is_dup = true;
		}
		else
		{
			ping->num_recv++;
			_set_seq_table(echo_data.echo_sequence, ping);
		}
		if (echo_data.echo_triptime > 0)
		_store_stats(&ping->stats, echo_data.echo_triptime);
	}
	else
		set_hostname_by_in_addr(echo_data.host, packet.iphdr.saddr);

	print_reply(&echo_data, ping->verbose, is_dup);
}