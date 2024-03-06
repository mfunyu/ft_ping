#include "ping_struct.h"
#include "ping_icmp.h"
#include "ping_error.h"
#include "ping_print.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static void	_store_stats(t_ping *ping, double triptime)
{
	ping->stats.recieved++;
	ping->stats.sum += triptime;
	ping->stats.sum_sq += calc_square(triptime);
	if (ping->stats.min == 0 || triptime < ping->stats.min)
		ping->stats.min = triptime;
	if (triptime > ping->stats.max)
		ping->stats.max = triptime;
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
	set_hostname_by_in_addr(echo_data->host, packet->iphdr.saddr);
	set_ip_by_in_addr(echo_data->ip, packet->iphdr.saddr);
	if (echo_data->type == ICMP_ECHOREPLY)
	{
		echo_data->echo_sequence = ntohs(packet->icmphdr.echo_sequence);
		echo_data->echo_ttl = packet->iphdr.ttl;
		echo_data->echo_triptime = _calc_triptime(packet, tv_recv);
	}
	else
	{
		echo_data->un.error.iphdr = packet->req_iphdr;
		echo_data->un.error.icmphdr = packet->req_icmphdr;
	}
}

static bool	_is_reply(t_packet *packet, int ident)
{
	/* ignore requests */
	if (packet->icmphdr.type == ICMP_ECHO)
		return (false);

	/* ignore responses to other ping */
	if (packet->icmphdr.type == ICMP_ECHOREPLY)
	{
		if (packet->icmphdr.echo_id != htons(ident))
			return (false);
	}
	else if (packet->req_icmphdr.echo_id != htons(ident))
		return (false);
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
			error_exit("recvmsg error");
	}
	return (ret);
}

void	ping_recv(t_ping *ping)
{
	t_packet	packet;
	ssize_t		ret;
	t_echo_data	echo_data;

	ret = _recv_reply(ping->sfd, &packet);
	if (ret < 0)
		return ;
	if (!_is_reply(&packet, ping->ident))
		return ;

	_set_echo_data(&echo_data, &packet, ret);
	if (echo_data.type == ICMP_ECHOREPLY)
	{
		if (!icmp_is_correct_checksum(&packet.icmphdr, echo_data.icmplen))
			return ;
		_store_stats(ping, echo_data.echo_triptime);
	}

	print_reply(&echo_data, ping->verbose);
}