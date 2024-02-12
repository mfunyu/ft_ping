#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include "ft_ping.h"

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

void	print_stats(t_icmp_send *send, struct timeval *tv)
{
	printf("%d bytes from %s: icmp_seq=%d", send->len, send->ip, send->seq++);
	printf(" ttl=%d", 0);
	_print_timetrip(&(send->tv), tv);
	printf("\n");
}

void	print_recv(ssize_t ret, struct msghdr *msg)
{
	(void)msg;
	switch (ret)
	{
		case -1:
			printf("timeout\n");
			break;
		default:
			printf("received %ld bytes\n", ret);
	}
}