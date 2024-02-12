#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include "ft_ping.h"
#include "error.h"

void	print_icmp_error()
{
	printf("IP Hdr Dump:\n");
	// TODO: print ip header dump
	return ;
}

static void	_print_timetrip(struct timeval *tv_start, struct timeval *tv_end)
{
	size_t	triptime;

	triptime = (tv_end->tv_sec - tv_start->tv_sec) * 1000 * 1000;
	triptime += tv_end->tv_usec - tv_start->tv_usec;

	printf(" time=%ld,%03ld ms", triptime / 1000, triptime % 1000);
}

static void	_print_stats(t_icmp_send *send, struct timeval *tv)
{
	printf(" icmp_seq=%d", send->seq++);
	printf(" ttl=%d", 0);
	_print_timetrip(&(send->tv), tv);
}

void	print_recv(ssize_t ret, struct msghdr *msg, t_icmp_send *send)
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		error_exit("gettimeofday error");
	(void)msg;

	printf("%d bytes from %s:", send->len, send->ip);
	switch (ret)
	{
		case -1:
			printf("timeout\n");
			break;
		default:
			_print_stats(send, &tv);
			printf(" (received %ld bytes)", ret);
	}
	printf("\n");
}