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

void	analyse_packet(ssize_t ret, struct msghdr *msg, t_icmp_recv *recv)
{
	struct icmphdr		*hdr;
	struct sockaddr_in	*src_addr;
	char				*content;

	content = msg->msg_iov->iov_base;
	hdr = (struct icmphdr *)(content + sizeof(struct iphdr));
	src_addr = (struct sockaddr_in*)msg->msg_name;

	recv->len = ret - sizeof(struct iphdr);
	memcpy(recv->ip, inet_ntoa(src_addr->sin_addr), INET_ADDRSTRLEN);
	getnameinfo((struct sockaddr *)src_addr, sizeof(struct sockaddr_in), recv->host, INET_ADDRSTRLEN, NULL, 0, 0);
	recv->type = hdr->type;

	printf("type: %d ", recv->type);
	printf("ip: %s ", recv->ip);
	printf("seq: %d\n", recv->seq);
	print_recv(msg, recv);
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

	ret = recvmsg(sfd, msg, 0);
	return (ret);
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	int				ret;
	struct msghdr	msg;
	t_icmp_recv		recv;

	ret = receive_packet(sfd, &msg);
	if (ret > 0)
	{
		if (gettimeofday(&(recv.tv), NULL))
			error_exit("gettimeofday error");
		recv.seq = send->seq++;
		recv.tv_ret = send->tv;
		analyse_packet(ret, &msg, &recv);
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