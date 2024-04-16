#include "ft_ping.h"
#include "ping_struct.h"
#include "ping_error.h"
#include "utils.h"
#include "parser.h"
#include <unistd.h>
#include <errno.h>

static int _create_socket(int ttl)
{
	int	sfd;

	(void)ttl;
	sfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sfd == -1)
	{
		if (errno == EPERM)
			error_exit("Lacking privilege for icmp socket.");
		error_exit_strerr("socket error");
	}
# ifdef BONUS
	if (setsockopt(sfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)))
		error_exit_strerr("setsockopt error");
# endif
	return (sfd);
}

void	ping_init(t_ping *ping, t_args *args)
{
	ping->dst_hostname = args->params[0];
	ping->datalen = PING_DEFAULT_DATALEN;
	ping->ttl = PING_DEFAULT_TTL;
# ifdef BONUS
	if (args->flags[SIZE] > -1)
		ping->datalen = args->flags[SIZE];
	ping->ping_count = args->flags[COUNT];
	if (args->flags[TTL])
		ping->ttl = args->flags[TTL];
# endif
	ping->icmplen = sizeof(struct icmphdr) + ping->datalen;
	ping->verbose = false;
	if (args->flags[VERBOSE])
		ping->verbose = true;

	set_sockaddr_by_hostname(&ping->dst_addr, ping->dst_hostname);
	set_ip_by_sockaddr(ping->dst_ip, &ping->dst_addr);
	ping->ident = getpid();
	ping->interval.tv_sec = 0;
	ping->interval.tv_usec = PING_DEFAULT_INTERVAL_S * 1000000;
	ping->sfd = _create_socket(ping->ttl);
}
