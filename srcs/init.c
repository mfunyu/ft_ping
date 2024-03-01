#include "ft_ping.h"
#include "error.h"
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

static void	_set_ip_by_sockaddr(char *ip, struct sockaddr const *addr)
{
	struct sockaddr_in const	*addr_in;
	char const					*ret;

	addr_in = (struct sockaddr_in const *)addr;
	ret = inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
	if (!ret)
		error_exit("inet_ntop error");
}

static void	_set_sockaddr_by_hostname(struct sockaddr *addr, char const *hostname)
{
	struct addrinfo	hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_RAW,
		.ai_protocol = IPPROTO_ICMP
	};
	struct addrinfo	*result;
	int				ret;

	ret = getaddrinfo(hostname, NULL, &hints, &result);
	if (ret)
	{
		if (ret == EAI_NONAME)
			error_exit("unknown host");
		error_exit_gai("getaddrinfo error", ret);
	}
	memcpy(addr, result->ai_addr, sizeof(struct sockaddr));
	freeaddrinfo(result);
}

static int _create_socket(void)
{
	int	sfd;

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd == -1)
		error_exit("socket error");
	return (sfd);
}

void	init(t_ping *ping, t_args *args)
{
	ping->dst_hostname = args->params[0];
	ping->len = ICMP_DEFAULT_PACKET_SIZE;
	if (args->flags[SIZE])
		ping->len = args->flags[SIZE] + sizeof(struct icmphdr);

	_set_sockaddr_by_hostname(&ping->dst_addr, ping->dst_hostname);
	_set_ip_by_sockaddr(ping->dst_ip, &ping->dst_addr);
	ping->ident = getpid();
	ping->sfd = _create_socket();
}
