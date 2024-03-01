#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "error.h"
#include "ft_ping.h"

int create_raw_socket(void)
{
	int	sfd;

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd == -1)
		error_exit("socket error");
	return (sfd);
}

void	cleanup(t_ping *ping)
{
	freeaddrinfo(ping->addr);
	close(ping->sfd);
}