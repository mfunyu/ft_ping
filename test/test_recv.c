#define _GNU_SOURCE
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/ip_icmp.h>

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
	static ssize_t	(*orig_recv)(int, void *, size_t, int);
	ssize_t			ret;

	if (!orig_recv)
	{
		orig_recv = dlsym(RTLD_NEXT, "recv");
		printf("--- recv: packet will be modified ---\n");
	}
	ret = orig_recv(sockfd, buf, len, flags);
	*(char *)(buf + ret - 1) = 'A';
	return (ret);
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
	static ssize_t	(*orig_recvfrom)(int, void *, size_t , int , struct sockaddr *, socklen_t *);
	ssize_t			ret;

	if (!orig_recvfrom)
	{
		orig_recvfrom = dlsym(RTLD_NEXT, "recvfrom");
		printf("--- recvfrom: packet will be modified ---\n");
	}
	ret = orig_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
	*(char *)(buf + ret - 1) = 'B';

	return (ret);
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
	static ssize_t	(*orig_recvmsg)(int, struct msghdr *, int);
	ssize_t			ret;

	if (!orig_recvmsg)
	{
		orig_recvmsg = dlsym(RTLD_NEXT, "recvmsg");
		printf("--- recvmsg: packet will be modified ---\n");
	}
	ret = orig_recvmsg(sockfd, msg, flags);
	if (ret > sizeof(struct iphdr) && msg->msg_iov->iov_len > 0)
		*(char *)(msg->msg_iov->iov_base + ret - 1) = 'C';
	return (ret);
};