#include "ft_ping.h"
#include "libft.h"
#include <stdio.h>

uint16_t	icmp_calc_checksum(char *msg, size_t len)
{
	size_t		i;
	uint32_t	sum;
	uint16_t	checksum;

	while (i < len / sizeof(uint16_t))
	{
		sum += (uint16_t)msg[i];
		if (sum > 0xffff)
			sum = (sum & 0xffff) + 1;
		i++;
	}
	checksum = ~sum;
	return (checksum);
}

void	icmp_add_data(char *msg, size_t len)
{
	size_t	i;

	i = sizeof(t_icmp_header);
	while (i < len)
	{
		msg[i] = '*';
		i++;
	}
}

void	icmp_resuqest_print(char *msg, size_t len)
{
	t_icmp_header	*header;
	size_t			i;

	header = (t_icmp_header *)msg;
	printf("type: %02x\n", header->type);
	printf("code: %02x\n", header->code);
	printf("checksum: %04x\n", header->checksum);
	printf("identifier: %04x\n", header->identifier);
	printf("sequence_number: %04x\n", header->sequence_number);
	printf("data: ");
	i = sizeof(t_icmp_header);
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
	t_icmp_header	header;

	header.type = ICMP_ECHO_REQUEST;
	header.code = 0;
	header.checksum = 0;
	header.identifier = 0;
	header.sequence_number = 0;

	ft_memcpy(msg, &header, sizeof(header));
	icmp_add_data(msg, len);
	header.checksum = icmp_calc_checksum(msg, len);
	ft_memcpy(msg, &header, sizeof(header));
	icmp_resuqest_print(msg, len);
}