#include "ft_ping.h"
#include "error.h"
#include <arpa/inet.h>
#include <netdb.h>


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


void	init_send(t_icmp_send *send, t_args *args)
{
	send->len = ICMP_DEFAULT_PACKET_SIZE;
	if (args->flags[SIZE])
		send->len = args->flags[SIZE] + sizeof(struct icmphdr);

	send->addr = host_to_addrinfo(args->params[0]);
	get_ip_addr(send->addr, send->ip);
}
