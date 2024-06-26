#include "ft_ping.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
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
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16); /* add carry */
	return (~sum); /* one's complement */
}

bool	icmp_is_correct_checksum(struct icmphdr *icmphdr, size_t icmplen)
{
	return (_icmp_calc_checksum((char *)icmphdr, icmplen) == 0);
}

void	icmp_add_checksum(char *msg, size_t len)
{
	struct icmphdr	*header;

	header = (struct icmphdr *)msg;
	header->checksum = _icmp_calc_checksum(msg, len);
}

void	icmp_set_data(char *msg, size_t total_len)
{
	size_t	offset;
	size_t	i;

	offset = sizeof(struct icmphdr) + sizeof(struct timeval);
	msg[total_len] = 0;
	i = 0;
	while (offset + i < total_len)
	{
		msg[offset + i] = i % 256;
		i++;
	}
}

void	icmp_add_timestamp(char *msg, size_t total_len)
{
	struct timeval	tv;

	tv = get_current_time();
	if (total_len < sizeof(struct icmphdr) + sizeof(struct timeval))
		return ;
	memcpy(msg + sizeof(struct icmphdr), &tv, sizeof(tv));
}

void	icmp_set_icmphdr(char *msg, int ident, int seqno)
{
	struct icmphdr	*header;

	header = (struct icmphdr *)msg;
	header->type = ICMP_ECHO;
	header->code = 0;
	header->checksum = 0;
	header->un.echo.id = htons(ident);
	header->un.echo.sequence = htons(seqno);
}