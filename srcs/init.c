#include "ft_ping.h"
#include "error.h"
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char	*get_ip_addr(struct addrinfo *addr, char *ip)
{
	struct sockaddr_in const *addr_in;
	const char	*ret;

	addr_in = (struct sockaddr_in const *)addr->ai_addr;
	ret = inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
	if (!ret)
		error_exit("inet_ntop error");
	return (ip);
}

struct addrinfo	*host_to_addrinfo(char const *hostname)
{
	int				s;
	struct addrinfo	*result;
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	s = getaddrinfo(hostname, NULL, &hints, &result);
	if (s != 0)
	{
		if (s == -2)
			error_exit("unknown host");
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	return (result);
}


void	init(t_ping *ping, t_args *args)
{
	ping->req_host = args->params[0];
	ping->len = ICMP_DEFAULT_PACKET_SIZE;
	if (args->flags[SIZE])
		ping->len = args->flags[SIZE] + sizeof(struct icmphdr);

	ping->addr = host_to_addrinfo(args->params[0]);
	get_ip_addr(ping->addr, ping->req_ip);
	ping->ident = getpid();
	ping->sfd = create_raw_socket();
}
