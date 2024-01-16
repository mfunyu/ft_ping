#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int dns_resolution(char const *hostname)
{
	int s;
	struct addrinfo *result;
	struct addrinfo *rp;
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_ICMP;

	s = getaddrinfo(hostname, NULL, &hints, &result);
	if (s != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		printf("Family: %d\n", rp->ai_family);
		printf("Socktype: %d\n", rp->ai_socktype);
		printf("Protocol: %d\n", rp->ai_protocol);
		printf("Addr len: %d\n", rp->ai_addrlen);
		//printf("Addr: %s\n", rp->ai_addr);
	}
	freeaddrinfo(result);

	if (rp == NULL)
	{
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}
	return (0);
}
