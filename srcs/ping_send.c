#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <netdb.h>
#include <sys/time.h>
#include "utils_time.h"

static void	_send_echo(char *msg, t_ping *ping)
{
	int	ret;

	ret = sendto(ping->sfd, msg, ping->len, 0, &ping->dst_addr, INET_ADDRSTRLEN);
	if (ret == -1)
		error_exit("sendto error");
}

void	ping_send(t_ping *ping)
{
	char	msg[ICMP_MAX_PACKET_SIZE];

	icmp_set_icmphdr(msg, ping->ident, ping->num_xmit);
	icmp_set_data(msg, ping->len);
	icmp_add_timestamp(msg);
	icmp_add_checksum(msg, ping->len);

	_send_echo(msg, ping);
	ping->num_xmit++;
	printf("ping\n");
}
