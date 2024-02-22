#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include "ft_ping.h"
#include "error.h"
#include <errno.h>
#include <stdio.h>
#include "print.h"
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

void	get_source_info(struct sockaddr_in *src_addr, t_icmp_recv *recv)
{
	int	ret;

	memcpy(recv->ip, inet_ntoa(src_addr->sin_addr), INET_ADDRSTRLEN);
	ret = getnameinfo((struct sockaddr *)src_addr, sizeof(struct sockaddr_in), recv->host, HOST_NAME_MAX, NULL, 0, 0);
	if (ret)
		printf("getnameinfo error: %s\n", gai_strerror(ret));
	printf("ip: %s \n", recv->ip);
}

void	analyse_response(t_packet *packet, t_icmp_recv *recv, ssize_t total)
{
	recv->type = packet->icmphdr.type;
	recv->sequence = packet->icmphdr.echo_sequence;
	recv->len = total - sizeof(struct iphdr);

	printf("type: %d ", recv->type);
}

void	calculate_timetrip(struct timeval *tv, t_icmp_send *send, t_icmp_recv *recv)
{
	recv->triptime = (tv->tv_sec - send->tv.tv_sec) * 1000 * 1000;
	recv->triptime += tv->tv_usec - send->tv.tv_usec;
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	t_packet		packet;
	struct sockaddr_in	src_addr;
	struct iovec		iov = {
		.iov_base = (void *)&packet,
		.iov_len = sizeof(t_packet)
	};
	struct msghdr	msg = {
		.msg_iov = &iov,
		.msg_iovlen = 1,
		.msg_name = &src_addr,
		.msg_namelen = sizeof(struct sockaddr_in),
	};
	ssize_t			ret;
	struct timeval	tv;
	t_icmp_recv		recv;

	ret = recvmsg(sfd, &msg, MSG_DONTWAIT);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		else
			error_exit("recvmsg error");
	}
	if (gettimeofday(&tv, NULL))
		error_exit("gettimeofday error");
	if (!_is_valid_packet(&packet))
		return ;
	analyse_response(&packet, &recv, ret);
	get_source_info(&src_addr, &recv);
	calculate_timetrip(&tv, send, &recv);
	print_recv(&msg, &recv);
}