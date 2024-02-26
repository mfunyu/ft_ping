#include "ft_ping.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>

static uint16_t	_icmp_calc_checksum(char *msg, size_t len)
{
	size_t		i;
	uint32_t	sum;
	uint16_t	*words;

	i = 0;
	sum = 0;
	words = (uint16_t *)msg;
	while (i < len / 2)
		sum += words[i++];
	if (len % 2)
		sum += words[len / 2];
	sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

void	icmp_add_checksum(char *msg, size_t len)
{
	struct icmphdr	*header;

	header = (struct icmphdr *)msg;
	header->checksum = _icmp_calc_checksum(msg, len);
}

void	icmp_set_data(char *msg, size_t total_len)
{
	size_t	start;
	size_t	i;

	start = sizeof(struct icmphdr) + sizeof(n_time);
	i = 0;
	while (start + i < total_len)
	{
		msg[start + i] = i % 256;
		i++;
	}
}

void	icmp_add_timestamp(char *msg)
{
	struct timeval	tv;
	n_time			otime;

	gettimeofday(&tv, NULL);
	otime = htonl((tv.tv_sec % (60 * 60 * 24)) * 1000 + tv.tv_usec / 1000);
	memcpy(msg + sizeof(struct icmphdr), &otime, sizeof(otime));
}

void	icmp_set_icmphdr(char *msg, size_t sequence)
{
	struct icmphdr	*header;

	header = (struct icmphdr *)msg;
	header->type = ICMP_ECHO;
	header->code = 0;
	header->checksum = 0;
	header->un.echo.id = getpid();
	header->un.echo.sequence = sequence;
}