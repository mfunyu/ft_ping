#include <stdio.h>
#include <string.h>
#include <netinet/ip_icmp.h>

static void	_print_icmp_error()
{
	printf("IP Hdr Dump:\n");
	// TODO: print ip header dump
	return ;
}

int	receive_packet(int sfd)
{
	char		buf[1024];
	char		addrbuf[1024];
	struct msghdr	msg;
	struct iovec	iov;
	ssize_t			ret;

	memset(&msg, 0, sizeof(msg));
	iov.iov_base = buf;
	iov.iov_len = 1024;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_name = addrbuf;
	msg.msg_namelen = sizeof(addrbuf);

	ret = recvmsg(sfd, &msg, MSG_DONTWAIT);
	if (ret == -1)
	{
		return (-1);
	}
	printf("received %ld bytes\n", ret);
	return (0);
}

void	handle_recv(int sfd)
{
	int				ret;

	while (1)
	{
		ret = receive_packet(sfd);
		if (ret >= 0)
			return ;
	}
	_print_icmp_error();
}