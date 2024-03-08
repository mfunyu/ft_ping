#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <limits.h>
#include "ping_error.h"

void	set_ip_by_sockaddr(char *ip, struct sockaddr const *addr)
{
	struct sockaddr_in const	*addr_in;
	char const					*ret;

	addr_in = (struct sockaddr_in const *)addr;
	ret = inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN);
	if (!ret)
		error_exit_strerr("inet_ntop error");
}

void	set_ip_by_in_addr(char *ip, in_addr_t saddr)
{
	char const	*ret;

	ret = inet_ntop(AF_INET, &saddr, ip, INET_ADDRSTRLEN);
	if (!ret)
		error_exit_strerr("inet_ntop error");
}

void	set_hostname_by_in_addr(char *hostname, in_addr_t saddr)
{
	int					ret;
	struct sockaddr_in	in = {
		.sin_family = AF_INET,
		.sin_addr = {
			.s_addr = saddr
		}
	};

	ret = getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in), hostname, HOST_NAME_MAX, NULL, 0, 0);
	if (ret)
		error_exit_gai("getnameinfo error", ret);
}

void	set_sockaddr_by_hostname(struct sockaddr *addr, char const *hostname)
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