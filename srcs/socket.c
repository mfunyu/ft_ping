#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "error.h"

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

int create_raw_socket(void)
{
	int	sfd;

	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd == -1)
		error_exit("socket error");
	return (sfd);
}

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

void	send_packet(struct addrinfo *addr, int sfd, char *msg, size_t len)
{
	static int	icmp_seq;
	char	ip[INET_ADDRSTRLEN];
	int		n;

	n = sendto(sfd, msg, len, 0, addr->ai_addr, addr->ai_addrlen);
	if (n == -1)
		error_exit("sendto error");
	printf("%ld bytes from %s: ", len, get_ip_addr(addr, ip));
	printf("icmp_seq=%d ttl=%d time=%d ms\n", icmp_seq++, 0, 0);
}

void	cleanup(struct addrinfo *addr, int sfd)
{
	freeaddrinfo(addr);
	close(sfd);
}