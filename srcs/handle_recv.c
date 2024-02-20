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

bool	analyse_packet(ssize_t total, struct msghdr *msg, t_icmp_recv *recv)
{
	struct icmphdr		*hdr;
	struct sockaddr_in	*src_addr;
	char				*content;
	int					ret;

	content = msg->msg_iov->iov_base;
	hdr = (struct icmphdr *)(content + sizeof(struct iphdr));
	recv->type = hdr->type;
	if (recv->type == ICMP_ECHO)
		return (false);

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
	t_icmp_recv		recv;

	ret = receive_packet(sfd, &msg);
	if (ret > 0)
	{
		if (gettimeofday(&(recv.tv), NULL))
			error_exit("gettimeofday error");
		if (!analyse_packet(ret, &msg, &recv))
			return ;
		recv.seq = send->seq++;
		recv.tv_ret = send->tv;
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