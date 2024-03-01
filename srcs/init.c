#include "ft_ping.h"
#include "error.h"
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

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
