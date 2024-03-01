#include "ft_ping.h"
#include "ping_struct.h"
#include "error.h"
#include <errno.h>
#include <stdio.h>
#include "print.h"
#include "utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

static bool	_is_valid_packet(t_packet *packet, int ident)
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

void	resolve_source_info(t_packet *packet, t_reply_data *r_data)
{
	const char			*result;
	int					ret;
	struct sockaddr_in	in = {
		.sin_family = AF_INET,
		.sin_addr = {
			.s_addr = packet->iphdr.saddr
		}
	};

	result = inet_ntop(AF_INET, &packet->iphdr.saddr, r_data->ip, INET_ADDRSTRLEN);
	if (result == NULL)
		error_exit("inet_ntop error");

	ret = getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in), r_data->host, HOST_NAME_MAX, NULL, 0, 0);
	if (ret)
		error_exit_gai("getnameinfo error", ret);
}

static void	store_stats(t_ping *ping, double triptime)
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
	printf("tv_request: %ld.%06ld\n", tv_request->tv_sec, tv_request->tv_usec);
	return (triptime);
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
	return (recvmsg(sfd, &msg, MSG_DONTWAIT));
}

void	ping_recv(t_ping *ping)
{
	t_packet		packet;
	ssize_t			ret;
	struct timeval	tv;
	t_reply_data	r_data;

	ret = _recv_reply(ping->sfd, &packet);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		error_exit("recvmsg error");
	}
	tv = get_current_time();
	if (!_is_valid_packet(&packet, ping->ident))
		return ;

	resolve_source_info(&packet, &r_data);
	if (packet.icmphdr.type == ICMP_ECHOREPLY)
	{
		if (icmp_calc_checksum((char *)&packet.icmphdr, ret - sizeof(struct iphdr)) != 0)
			return ;
		if (strncmp(ping->dst_ip, r_data.ip, INET_ADDRSTRLEN) != 0)
			return ;
	}

	r_data.len = ret - sizeof(struct iphdr);
	r_data.type = packet.icmphdr.type;
	r_data.ttl = packet.iphdr.ttl;
	r_data.sequence = ntohs(packet.icmphdr.echo_sequence);
	r_data.triptime = _calc_triptime(&packet, tv);
	if (r_data.type == ICMP_ECHOREPLY)
		store_stats(ping, r_data.triptime);
	print_reply(&r_data);
}