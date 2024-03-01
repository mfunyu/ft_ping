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

static void	_print_stats(t_echo_data *echo_data)
{
	printf(": icmp_seq=%d", echo_data->sequence);
	printf(" ttl=%d", echo_data->ttl);
	printf(" time=%.3f ms", echo_data->triptime);
}

void	print_reply(t_echo_data *echo_data)
{
	printf("%d bytes from", echo_data->icmplen);
	switch (echo_data->type)
	{
	case ICMP_ECHOREPLY:
		printf(" %s", echo_data->ip);
		_print_stats(echo_data);
		break;
	case ICMP_DEST_UNREACH:
		printf(" %s (%s)", echo_data->host, echo_data->ip);
		printf(": Destination Host Unreachable");
		break;
	case ICMP_TIME_EXCEEDED:
		printf(" %s (%s)", echo_data->host, echo_data->ip);
		printf(": Time to live exceeded");
		break;
	}
	printf("\n");
}