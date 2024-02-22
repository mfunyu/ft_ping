#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include "ft_ping.h"
#include "error.h"
#include <errno.h>
#include <stdio.h>
#include "print.h"
#include "utils.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

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

void	resolve_source_info(t_packet *packet, t_icmp_recv *recv)
{
	const char			*result;
	int					ret;
	struct sockaddr_in	in = {
		.sin_family = AF_INET,
		.sin_addr = {
			.s_addr = packet->iphdr.saddr
		}
	};

	result = inet_ntop(AF_INET, &packet->iphdr.saddr, recv->ip, INET_ADDRSTRLEN);
	if (result == NULL)
		error_exit("inet_ntop error");

	ret = getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in), recv->host, HOST_NAME_MAX, NULL, 0, 0);
	if (ret)
		printf("getnameinfo error: %s\n", gai_strerror(ret));
	printf("ip: %s \n", recv->ip);
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

void	handle_recv(int sfd, t_icmp_send *send)
{
	t_packet		packet;
	ssize_t			ret;
	t_icmp_recv		recv;

	ret = _recv_reply(sfd, &packet);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		error_exit("recvmsg error");
	}
	recv.tv_recv = get_current_timestamp();
	if (!_is_valid_packet(&packet))
		return ;

	recv.len = ret - sizeof(struct iphdr);
	recv.type = packet.icmphdr.type;
	recv.sequence = packet.icmphdr.echo_sequence;
	resolve_source_info(&packet, &recv);
	recv.triptime = calc_timetrip(&send->tv, &recv.tv_recv);

	print_recv(&recv);
}