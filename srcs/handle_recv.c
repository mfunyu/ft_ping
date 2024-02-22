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

bool	analyse_packet(ssize_t total, struct msghdr *msg, t_icmp_recv *recv)
{
	struct sockaddr_in	*src_addr;
	t_packet			*packet;
	int					ret;

	packet = (t_packet *)msg->msg_iov->iov_base;
	if (packet->icmphdr.type == ICMP_ECHO)
		return (false);
	recv->type = packet->icmphdr.type;
	if (packet->icmphdr.type != ICMP_ECHOREPLY)
	{
		if (packet->req_icmphdr.echo_id != getpid())
			return (false);
	}
	else if (packet->icmphdr.echo_id != getpid())
		return (false);

	recv->sequence = packet->icmphdr.echo_sequence;
	src_addr = (struct sockaddr_in*)msg->msg_name;

	recv->len = total - sizeof(struct iphdr);
	memcpy(recv->ip, inet_ntoa(src_addr->sin_addr), INET_ADDRSTRLEN);
	ret = getnameinfo((struct sockaddr *)src_addr, sizeof(struct sockaddr_in), recv->host, HOST_NAME_MAX, NULL, 0, 0);
	if (ret)
		printf("getnameinfo error: %s\n", gai_strerror(ret));

	printf("type: %d ", recv->type);
	printf("ip: %s \n", recv->ip);
	return (true);
}

int	receive_packet(int sfd, struct msghdr *msg)
{
	char				buf[1024];
	struct sockaddr_in	src_addr;
	struct iovec		iov;
	ssize_t				ret;

	memset(msg, 0, sizeof(*msg));
	iov.iov_base = buf;
	iov.iov_len = 1024;
	msg->msg_iov = &iov;
	msg->msg_iovlen = 1;
	msg->msg_name = &src_addr;
	msg->msg_namelen = sizeof(src_addr);

	ret = recvmsg(sfd, msg, MSG_DONTWAIT);
	return (ret);
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	int				ret;
	struct msghdr	msg;
	struct timeval	tv;
	t_icmp_recv		recv;

	ret = receive_packet(sfd, &msg);
	if (ret > 0)
	{
		if (gettimeofday(&tv, NULL))
			error_exit("gettimeofday error");
		if (!analyse_packet(ret, &msg, &recv))
			return ;
		recv.triptime = (tv.tv_sec - send->tv.tv_sec) * 1000 * 1000;
		recv.triptime += tv.tv_usec - send->tv.tv_usec;
		print_recv(&msg, &recv);
		return ;
	}
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return ;
		else
			error_exit("recvmsg error");
	}
}