#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <netdb.h>

static void	_send_packet(int sfd, t_icmp_send *send)
{
	static int	seq = 0;
	int			ret;

	ret = sendto(sfd, send->msg, send->len, 0, send->addr->ai_addr, send->addr->ai_addrlen);
	if (ret == -1)
		error_exit("sendto error");
	printf("%ld bytes from %s: ", send->len, send->ip);
	printf("icmp_seq=%d ttl=%d time=%d ms\n", seq++, 0, 0);
}

void	handle_send(int sfd, t_icmp_send *send)
{
	_send_packet(sfd, send);
}

