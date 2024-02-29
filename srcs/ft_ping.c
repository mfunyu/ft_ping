#include "ft_ping.h"
#include "error.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>

e_status g_status = NO_STATUS;

void	help(void)
{
	printf("Usage: ./ft_ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf(" Options valid:\n\
  -v, --verbose              verbose output\n");
# ifdef BONUS
	printf("\
  -c, --count=NUMBER         stop after sending NUMBER packets\n");
	printf("\
  -t, --ttl=N                specify N as time-to-live\n");
	printf("\
  -s, --size=NUMBER          ping NUMBER data octets\n");
# endif
	printf("\n\
  -?, --help                 give this help list\n\n");
	printf("Mandatory or optional arguments to long options are also mandatory or optional\n\
for any corresponding short options.\n\n");
	printf("Report bugs to: <htttps://github.com/mfunyu/ft_ping/issues>\n");
}

void	sig_int(int sig)
{
	(void)sig;
	g_status = INTERRUPT;
}

struct timeval	_set_timeout(struct timeval last)
{
	struct timeval	now;
	struct timeval	timeout;
	struct timeval	interval = {
		.tv_sec = 0,
		.tv_usec = 1000000,
	};

	get_current_timestamp(&now);
	timeout = calc_time_sub(last, now);
	timeout = calc_time_sub(timeout, interval);
	// printf("now: %ld.%06ld\n", now.tv_sec, now.tv_usec);
	// printf("last: %ld.%06ld\n", last.tv_sec, last.tv_usec);
	//printf("timeout: %ld.%06ld\n", timeout.tv_sec, timeout.tv_usec);
	return (timeout);
}

void	main_loop(int sfd, t_ping *ping)
{
	struct timeval	timeout;
	struct timeval	last;
	int 			ready;
	fd_set			readfds;

	handle_request(sfd, ping);
	get_current_timestamp(&last);
	while (g_status != INTERRUPT)
	{
		FD_ZERO(&readfds);
		FD_SET(sfd, &readfds);
		timeout = _set_timeout(last);
		ready = select(sfd + 1, &readfds, NULL, NULL, &timeout);
		if (ready < 0)
		{
			if (errno != EINTR)
				error_exit("select");
			printf("Interrupted system call\n");
		}
		else if (ready)
			handle_reply(sfd, ping);
		else
		{
			handle_request(sfd, ping);
			get_current_timestamp(&last);
		}
	}
}

void	print_footer(t_ping *ping)
{
	double	avg;
	double	variance;

	printf("--- %s ping statistics ---\n", ping->req_host);
	printf("%zu packets transmitted, %zu packets received, %zu%% packet loss\n",
		ping->num_xmit, ping->stats.recieved, (ping->num_xmit - ping->stats.recieved) * 100 / ping->num_xmit);
	if (ping->stats.recieved == 0)
		return ;

	avg = ping->stats.sum / ping->stats.recieved;
	variance = ping->stats.sum_sq / ping->stats.recieved - avg * avg;
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		ping->stats.min, avg, ping->stats.max, calc_sqrt(variance, 0.0005));
}

void	ft_ping(t_args *args)
{
	int		sfd;
	t_ping	ping;

	sfd = create_raw_socket();
	init(&ping, args);
	signal(SIGINT, &sig_int);
	printf("PING %s (%s): %ld data bytes\n", ping.req_host, ping.req_ip, ping.len - sizeof(struct icmphdr));
	main_loop(sfd, &ping);
	print_footer(&ping);
	cleanup(ping.addr, sfd);
}

void	check_arguments(t_args *args, int ac, char **av)
{
	if (ac <= 1)
		error_exit_usage("missing host operand");
	parse_args(args, ac, av);
	if (args->flags[HELP])
	{
		help();
		exit(EXIT_SUCCESS);
	}
	if (!args->params[0])
		error_exit_usage("missing host operand");
	//print_args(args);
}

int	main(int ac, char **av)
{
	t_args	args = {0};

	check_arguments(&args, ac, av);
	ft_ping(&args);
	return (0);
}
