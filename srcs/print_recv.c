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

static void	_print_stats(t_icmp_recv *recv)
{
	printf(": icmp_seq=%d", recv->seq);
	printf(" ttl=%d", 0);
	printf(" time=%ld,%03ld ms", recv->triptime / 1000, recv->triptime % 1000);
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