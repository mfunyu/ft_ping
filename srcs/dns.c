#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int dns_resolution(char const *hostname)
{
	int s;
	int sfd;
	struct addrinfo *result;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	s = getaddrinfo(hostname, NULL, &hints, &result);
	if (s != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	if (result == NULL)
	{
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}
		printf("Family: %d\n", result->ai_family);
		printf("Socktype: %d\n", result->ai_socktype);
		printf("Protocol: %d\n", result->ai_protocol);
		printf("Addr len: %d\n", result->ai_addrlen);
		sfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_ICMP);
		if (sfd == -1)
			exit(EXIT_FAILURE);
		if (connect(sfd, result->ai_addr, result->ai_addrlen) == -1)
			exit(EXIT_SUCCESS);
	freeaddrinfo(result);
	printf("Connected\n");
	return (0);
}
