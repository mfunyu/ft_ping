#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
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

	icmp_echo_request_message(msg, ping->len);
	_send_request(sfd, msg, ping);
	ping->tv_request = get_current_timestamp();
	printf("ping\n");
	alarm(1);
	g_status = NO_STATUS;
}

