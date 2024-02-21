#include "ft_ping.h"
#include "libft.h"
#include <stdio.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>

uint16_t	icmp_calc_checksum(char *msg, size_t len)
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

void	icmp_add_data(char *msg, size_t len)
{
	size_t	i;

	i = sizeof(struct icmphdr);
	while (i < len)
	{
		msg[i] = '*';
		i++;
	}
}

void	icmp_resuqest_print(char *msg, size_t len)
{
	struct icmphdr	*header;
	size_t			i;

	header = (struct icmphdr *)msg;
	printf("type: %02x\n", header->type);
	printf("code: %02x\n", header->code);
	printf("checksum: %04x\n", header->checksum);
	printf("un.echo.id: %04x\n", header->un.echo.id);
	printf("un.echo.sequence: %04x\n", header->un.echo.sequence);
	printf("data: ");
	i = sizeof(struct icmphdr);
	while (i < len)
	{
		printf("%c", msg[i]);
		i++;
	}
	printf("\n");
	i = 0;
	while (i < len)
	{
		printf("%02x ", msg[i]);
		i++;
	}
	printf("\n");
}

void	icmp_echo_request_message(char *msg, size_t len)
{
	static int		sequence = 0;
	static int		id;
	struct icmphdr	header;

	if (!id)
		id = getpid();
	header.type = ICMP_ECHO;
	header.code = 0;
	header.checksum = 0;
	header.un.echo.id = id;
	header.un.echo.sequence = sequence++;

	ft_memcpy(msg, &header, sizeof(header));
	icmp_add_data(msg, len);
	header.checksum = icmp_calc_checksum(msg, len);
	ft_memcpy(msg, &header, sizeof(header));
	//icmp_resuqest_print(msg, len);
}