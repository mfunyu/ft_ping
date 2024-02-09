#include <stdio.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include "ft_ping.h"
#include "error.h"

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

static void	_print_timetrip(struct timeval *tv_start, struct timeval *tv_end)
{
	size_t	triptime;

	triptime = (tv_end->tv_sec - tv_start->tv_sec) * 1000 * 1000;
	triptime += tv_end->tv_usec - tv_start->tv_usec;

	if (triptime >= 100000)
		printf(" time=%ld ms", triptime / 1000);
	else if (triptime >= 10000)
		printf(" time=%ld.%01ld ms", triptime / 1000, triptime % 1000 / 100);
	else if (triptime >= 1000)
		printf(" time=%ld.%02ld ms", triptime / 1000, triptime % 1000 / 10);
	else
		printf(" time=%ld.%03ld ms", triptime / 1000, triptime % 1000);
}

static void	_print_stats(t_icmp_send *send, struct timeval *tv)
{
	printf("%d bytes from %s: icmp_seq=%d", send->len, send->ip, send->seq++);
	printf(" ttl=%d", 0);
	_print_timetrip(&(send->tv), tv);
	printf("\n");
}

void	handle_recv(int sfd, t_icmp_send *send)
{
	int				ret;
	struct timeval	tv;

	while (1)
	{
		ret = receive_packet(sfd);
		if (ret >= 0)
		{
			if (gettimeofday(&tv, NULL))
				error_exit("gettimeofday error");
			_print_stats(send, &tv);
			return ;
		}
	}
	_print_icmp_error();
}