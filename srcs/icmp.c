#include "ft_ping.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

static void	_icmp_set_data(char *msg, size_t start, size_t total_len)
{
	size_t	i;

	i = 0;
	while (start + i < total_len)
	{
		msg[start + i] = i % 256;
		i++;
	}
}

void	icmp_add_checksum(char *msg, size_t len)
{
	struct icmphdr	*header;

	header = (struct icmphdr *)msg;
	header->checksum = _icmp_calc_checksum(msg, len);
}

void	icmp_create_requestmsg(char *msg, size_t len, int sequence)
{
	struct icmphdr	header = {
		.type = ICMP_ECHO,
		.code = 0,
		.checksum = 0,
		.un.echo.id = getpid(),
		.un.echo.sequence = sequence,
	};

	memcpy(msg, &header, sizeof(header));
	_icmp_set_data(msg, sizeof(struct icmphdr), len);
}