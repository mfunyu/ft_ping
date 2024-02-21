#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

static void	_send_packet(int sfd, char *msg, t_icmp_send *send)
{
	int	ret;

	ret = sendto(sfd, msg, send->len, 0, send->addr->ai_addr, send->addr->ai_addrlen);
	if (ret == -1)
		error_exit("sendto error");
}

void	handle_send(int sfd, t_icmp_send *send)
{
	char	msg[ICMP_MAX_PACKET_SIZE];

	icmp_echo_request_message(msg, send->len);
	_send_packet(sfd, msg, send);
	if (gettimeofday(&send->tv, NULL))
		error_exit("gettimeofday error");
	printf("send\n");
	alarm(1);
	g_status = NO_STATUS;
}

