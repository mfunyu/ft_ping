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

volatile bool	g_stop = false;

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
	g_stop = true;
}

struct timeval	_set_timeout(struct timeval last)
{
	struct timeval	now;
	struct timeval	timeout;
	struct timeval	interval = {
		.tv_sec = 0,
		.tv_usec = PING_DEFAULT_INTERVAL_S * 1000000,
	};

	get_current_timestamp(&now);
	timeout = calc_time_sub(last, now);
	timeout = calc_time_sub(timeout, interval);
	return (timeout);
}

void	main_loop(t_ping *ping)
{
	struct timeval	interval;
	struct timeval	last;
	int 			ready;
	fd_set			readfds;

	handle_request(ping);
	get_current_timestamp(&last);
	while (!g_stop)
	{
		FD_ZERO(&readfds);
		FD_SET(ping->sfd, &readfds);
		interval = _set_timeout(last);
		ready = select(ping->sfd + 1, &readfds, NULL, NULL, &interval);
		if (ready < 0)
		{
			if (errno != EINTR)
				error_exit("select");
		}
		else if (ready)
			handle_reply(ping);
		else
		{
			handle_request(ping);
			get_current_timestamp(&last);
		}
	}
}

void	print_footer(t_ping *ping)
{
	double	avg;
	double	variance;

	printf("--- %s ping statistics ---\n", ping->dst_hostname);
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
	t_ping	ping = {0};

	init(&ping, args);
	signal(SIGINT, &sig_int);
	printf("PING %s (%s): %ld data bytes\n", ping.dst_hostname, ping.dst_ip, ping.len - sizeof(struct icmphdr));
	main_loop(&ping);
	print_footer(&ping);
	cleanup(&ping);
}

void	handle_args(t_args *args, int ac, char **av)
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
}

int	main(int ac, char **av)
{
	t_args	args = {0};

	handle_args(&args, ac, av);
	ft_ping(&args);
	return (0);
}
