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

int	receive_packet(int sfd, struct sockaddr_in *src_addr, struct msghdr *msg)
{
	char				buf[1024];
	struct iovec		iov = {
		.iov_base = buf,
		.iov_len = 1024
	};
	ssize_t				ret;

	memset(msg, 0, sizeof(*msg));
	msg->msg_iov = &iov;
	msg->msg_iovlen = 1;
	msg->msg_name = src_addr;
	msg->msg_namelen = sizeof(src_addr);

	ret = recvmsg(sfd, msg, MSG_DONTWAIT);
	return (ret);
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	int				ret;
	struct msghdr	msg;
	struct sockaddr_in	src_addr;
	struct timeval	tv;
	t_icmp_recv		recv;

	ret = receive_packet(sfd, &src_addr, &msg);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		else
			error_exit("recvmsg error");
	}
	if (gettimeofday(&tv, NULL))
		error_exit("gettimeofday error");
	if (!_is_valid_packet((t_packet *)msg.msg_iov->iov_base))
		return ;
	analyse_response((t_packet *)msg.msg_iov->iov_base, &recv, ret);
	get_source_info((struct sockaddr_in *)msg.msg_name, &recv);
	calculate_timetrip(&tv, send, &recv);
	print_recv(&msg, &recv);
}