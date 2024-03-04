#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include "ft_ping.h"
#include "ping_struct.h"
#include "ping_error.h"

struct icmp_desc
{
	int		type;
	int		code;
	char	*diag;
} icmp_desc[] = {
	{ICMP_DEST_UNREACH, ICMP_HOST_UNREACH, "Destination Host Unreachable"},
	{ICMP_TIME_EXCEEDED, ICMP_EXC_TTL, "Time to live exceeded"},
};

static void	_print_icmp_code(int type, int code)
{
	size_t				i;

	i = 0;
	while (i < sizeof(icmp_desc) / sizeof(icmp_desc[0]))
	{
		if (icmp_desc[i].type == type && icmp_desc[i].code == code)
		{
			printf("%s", icmp_desc[i].diag);
			return ;
		}
		i++;
	}
	printf ("Unknown Code: %d", code);
}

void	print_icmp_error()
{
	printf("IP Hdr Dump:\n");
	// TODO: print ip header dump
	return ;
}

static void	_print_stats(t_echo_data *echo_data)
{
	printf(": icmp_seq=%d", echo_data->echo_sequence);
	printf(" ttl=%d", echo_data->echo_ttl);
	printf(" time=%.3f ms", echo_data->echo_triptime);
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
	default:
		printf(" %s (%s) : ", echo_data->host, echo_data->ip);
		_print_icmp_code(echo_data->type, echo_data->code);
		break;
	}
	printf("\n");
}