#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include "ft_ping.h"
#include "ping_struct.h"
#include "error.h"

void	print_icmp_error()
{
	printf("IP Hdr Dump:\n");
	// TODO: print ip header dump
	return ;
}

static void	_print_stats(t_reply_data *r_data)
{
	printf(": icmp_seq=%d", r_data->sequence);
	printf(" ttl=%d", r_data->ttl);
	printf(" time=%.3f ms", r_data->triptime);
}

void	print_reply(t_reply_data *r_data)
{
	printf("%d bytes from", r_data->len);
	switch (r_data->type)
	{
		case ICMP_ECHOREPLY:
			printf(" %s", r_data->ip);
			_print_stats(r_data);
			break;
		case ICMP_DEST_UNREACH:
			printf(" %s (%s)", r_data->host, r_data->ip);
			printf(": Destination Host Unreachable");
			break;
		case ICMP_TIME_EXCEEDED:
			printf(" %s (%s)", r_data->host, r_data->ip);
			printf(": Time to live exceeded");
			break;
	}
	printf("\n");
}