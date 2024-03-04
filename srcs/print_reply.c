#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <arpa/inet.h>
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
	for (size_t i = 0; i < sizeof(icmp_desc) / sizeof(icmp_desc[0]); i++)
	{
		if (icmp_desc[i].type == type && icmp_desc[i].code == code)
		{
			printf("%s\n", icmp_desc[i].diag);
			return ;
		}
	}
	printf ("Unknown Code: %d\n", code);
}

static void	_print_ip_header(struct iphdr *iphdr)
{
	uint8_t	*ptr;

	printf("IP Hdr Dump:\n");
	ptr = (uint8_t *)iphdr;
	for (size_t i = 0; i < sizeof(struct iphdr); i++)
	{
		printf("%02x", *(ptr + i));
		if (i % 2)
			printf(" ");
	}
	printf("\n");
	printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src\tDst\tData\n");
	printf(" %1x  %1x  %02x", iphdr->version, iphdr->ihl, iphdr->tos);
	printf(" %04x", ntohs(iphdr->tot_len));
	printf(" %04x", ntohs(iphdr->id));
	printf("   %1x", ntohs(iphdr->frag_off) >> FRAGMENT_OFFSET_BITS);
	printf(" %04x", FRAGMENT_OFFSET(ntohs(iphdr->frag_off)));
	printf("  %02x", iphdr->ttl);
	printf("  %02x", iphdr->protocol);
	printf(" %04x", ntohs(iphdr->check));
	printf(" %s ", inet_ntoa(*(struct in_addr *)&iphdr->saddr));
	printf(" %s ", inet_ntoa(*(struct in_addr *)&iphdr->daddr));
	printf("\n");
}

static void	_print_ip_data(struct iphdr *iphdr, struct icmphdr *icmphdr)
{
	int		hdr_len;

	_print_ip_header(iphdr);
	printf("ICMP:");
	printf(" type %u", icmphdr->type);
	printf(", code %u", icmphdr->code);
	hdr_len = iphdr->ihl << IPHDR_ALIGNMENT_BITS;
	printf(", size %u", ntohs(iphdr->tot_len) - hdr_len);
	printf(", id 0x%04x", ntohs(icmphdr->un.echo.id));
	printf(", seq 0x%04x", ntohs(icmphdr->un.echo.sequence));
	printf("\n");
}

static void	_print_stats(t_echo_data *echo_data)
{
	printf(": icmp_seq=%d", echo_data->echo_sequence);
	printf(" ttl=%d", echo_data->echo_ttl);
	printf(" time=%.3f ms", echo_data->echo_triptime);
	printf("\n");
}

void	print_reply(t_echo_data *echo_data, bool verbose)
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
		if (verbose)
			_print_ip_data(&echo_data->req_iphdr, &echo_data->req_icmphdr);
		break;
	}
}