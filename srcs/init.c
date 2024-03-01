#include "ft_ping.h"
#include "error.h"
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

char	*get_ip_addr(struct sockaddr *addr, char *ip)
{
	struct sockaddr_in const *addr_in;
	const char	*ret;

	addr_in = (struct sockaddr_in const *)addr;
	ret = inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
	if (!ret)
		error_exit("inet_ntop error");
	return (ip);
}

static struct sockaddr	_get_sockaddr_by_hostname(char const *hostname)
{
	int				ret;
	struct addrinfo	*result;
	struct sockaddr	addr;
	struct addrinfo	hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_RAW,
		.ai_protocol = IPPROTO_ICMP
	};

	ret = getaddrinfo(hostname, NULL, &hints, &result);
	if (ret)
	{
		if (ret == EAI_NONAME)
			error_exit("unknown host");
		error_exit_gai("getaddrinfo error", ret);
	}
	memcpy(&addr, result->ai_addr, sizeof(struct sockaddr));
	freeaddrinfo(result);
	return (addr);
}

void	init(t_ping *ping, t_args *args)
{
	ping->dst_hostname = args->params[0];
	ping->len = ICMP_DEFAULT_PACKET_SIZE;
	if (args->flags[SIZE])
		ping->len = args->flags[SIZE] + sizeof(struct icmphdr);

	ping->dst_addr = _get_sockaddr_by_hostname(args->params[0]);
	get_ip_addr(&ping->dst_addr, ping->dst_ip);
	ping->ident = getpid();
	ping->sfd = create_raw_socket();
}
