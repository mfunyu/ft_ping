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

static void	_print_stats(t_icmp_recv *recv)
{
	printf(": icmp_seq=%d", recv->seq);
	printf(" ttl=%d", 0);
	_print_timetrip(&(recv->tv_ret), &(recv->tv));
}

void	print_recv(struct msghdr *msg, t_icmp_recv *recv)
{
	(void)msg;

	printf("%d bytes from", recv->len);
	switch (recv->type)
	{
		case ICMP_ECHOREPLY:
			printf(" %s", recv->ip);
			_print_stats(recv);
			break;
		case ICMP_DEST_UNREACH:
			printf(" %s (%s)", recv->host, recv->ip);
			printf(": Destination Host Unreachable");
			break;
		case ICMP_TIME_EXCEEDED:
			printf(" %s (%s)", recv->host, recv->ip);
			printf(": Time to live exceeded");
			break;
	}
	printf("\n");
}