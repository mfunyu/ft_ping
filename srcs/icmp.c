#include "ft_ping.h"
#include "libft.h"

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
}