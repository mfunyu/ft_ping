#include "ft_ping.h"
#include "error.h"
#include <errno.h>
#include <stdio.h>
#include "print.h"
#include "utils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>

static bool	_is_valid_packet(t_packet *packet)
{
	static int	pid;

	if (pid == 0)
		pid = getpid();

	/* ignore requests */
	if (packet->icmphdr.type == ICMP_ECHO)
		return (false);

	/* ignore responses to other ping */
	if (packet->icmphdr.type == ICMP_ECHOREPLY)
	{
		if (packet->icmphdr.echo_id != pid)
			return (false);
	}
	else if (packet->req_icmphdr.echo_id != pid)
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

void	handle_reply(int sfd, t_ping *ping)
{
	t_packet		packet;
	ssize_t			ret;
	t_reply_data	r_data;
	struct timeval	*tv_reply;

	ret = _recv_reply(sfd, &packet);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		error_exit("recvmsg error");
	}
	tv_reply = get_current_timestamp();
	if (!_is_valid_packet(&packet))
		return ;

	r_data.len = ret - sizeof(struct iphdr);
	r_data.type = packet.icmphdr.type;
	r_data.ttl = packet.iphdr.ttl;
	r_data.sequence = packet.icmphdr.echo_sequence;
	resolve_source_info(&packet, &r_data);
	r_data.triptime = calc_timetrip(ping, tv_reply);
	free(tv_reply);
	print_reply(&r_data);
}