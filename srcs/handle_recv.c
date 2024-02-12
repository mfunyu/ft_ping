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

void	analyse_packet(ssize_t ret, struct msghdr *msg, t_icmp_send *send)
{
	char *buf = msg->msg_iov->iov_base;

	printf("addrbuf: %s\n", buf);
	(void)ret;
	struct icmphdr *hdr;
	hdr = (struct icmphdr *)(buf + sizeof(struct iphdr));
	printf("type: %02x\n", hdr->type);
	printf("msg %s\n", inet_ntoa(((struct sockaddr_in*)msg->msg_name)->sin_addr));

	print_recv(hdr->type, msg, send);
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

	ret = receive_packet(sfd, &msg);
	if (ret > 0)
	{
		analyse_packet(ret, &msg, send);
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