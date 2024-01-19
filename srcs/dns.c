#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "error.h"

struct addrinfo	*host_to_addrinfo(char const *hostname)
{
	int s;
	struct addrinfo *result;
	struct addrinfo hints;

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
	printf("Family: %d\n", result->ai_family);
	printf("Socktype: %d\n", result->ai_socktype);
	printf("Protocol: %d\n", result->ai_protocol);
	printf("Addr len: %d\n", result->ai_addrlen);
	return (result);
}

int create_raw_socket(void)
{
	int sfd;

	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if (sfd == -1)
		error_exit("socket");
	return (sfd);
}

void	send_packet(struct addrinfo *addr, int sfd)
{
	int n;

	printf("Sending packet...\n");
	n = sendto(sfd, "hello", 5, 0, addr->ai_addr, addr->ai_addrlen);
	if (n == -1)
		error_exit("sendto");
}

void	cleanup(struct addrinfo *addr, int sfd)
{
	freeaddrinfo(addr);
	close(sfd);
}