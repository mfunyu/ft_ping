#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include "ft_ping.h"
#include "error.h"
#include <errno.h>
#include "print.h"

int	receive_packet(int sfd, struct msghdr *msg)
{
	char		buf[1024];
	char		addrbuf[1024];
	struct iovec	iov;
	ssize_t			ret;

	memset(msg, 0, sizeof(*msg));
	iov.iov_base = buf;
	iov.iov_len = 1024;
	msg->msg_iov = &iov;
	msg->msg_iovlen = 1;
	msg->msg_name = addrbuf;
	msg->msg_namelen = sizeof(addrbuf);

	ret = recvmsg(sfd, msg, 0);
	if (ret < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (-1);
		else
			error_exit("recvmsg error");
		return (-1);
	}
	print_recv(ret, msg);
	return (ret);
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	int				ret;
	struct timeval	tv;
	struct msghdr	msg;

	ret = receive_packet(sfd, &msg);
	if (ret > 0)
	{
		// TOD: print something
		if (gettimeofday(&tv, NULL))
			error_exit("gettimeofday error");
		print_stats(send, &tv);
		return ;
	}
}