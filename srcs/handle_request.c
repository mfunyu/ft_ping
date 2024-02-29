#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <netdb.h>
#include <sys/time.h>
#include "utils.h"

static void	_send_request(int sfd, char *msg, t_ping *ping)
{
	int	ret;

	ret = sendto(sfd, msg, ping->len, 0, ping->addr->ai_addr, ping->addr->ai_addrlen);
	if (ret == -1)
		error_exit("sendto error");
}

void	handle_request(int sfd, t_ping *ping)
{
	char	msg[ICMP_MAX_PACKET_SIZE];

	icmp_set_icmphdr(msg, ping->ident, ping->num_xmit);
	icmp_set_data(msg, ping->len);
	icmp_add_timestamp(msg);
	icmp_add_checksum(msg, ping->len);

	_send_request(sfd, msg, ping);
	ping->num_xmit++;
	printf("ping\n");
}

