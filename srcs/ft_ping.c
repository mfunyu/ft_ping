#include "ft_ping.h"
#include "error.h"
#include "utils.h"
#include "print.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>

volatile bool	g_stop = false;

void	sig_int(int sig)
{
	(void)sig;
	g_stop = true;
}

static void	_set_timeout(struct timeval *timeout, struct timeval last)
{
	struct timeval	now;
	struct timeval	tmp;
	struct timeval	interval = {
		.tv_sec = 0,
		.tv_usec = PING_DEFAULT_INTERVAL_S * 1000000,
	};

	now = get_current_time();
	tmp = calc_time_sub(last, now);
	*timeout = calc_time_sub(tmp, interval);
}

static void	_ping_run(t_ping *ping)
{
	struct timeval	interval;
	struct timeval	last;
	int 			ready;
	fd_set			readfds;

	print_header(*ping);
	ping_send(ping);
	last = get_current_time();
	while (!g_stop)
	{
		FD_ZERO(&readfds);
		FD_SET(ping->sfd, &readfds);
		_set_timeout(&interval, last);
		ready = select(ping->sfd + 1, &readfds, NULL, NULL, &interval);
		if (ready < 0)
		{
			if (errno != EINTR)
				error_exit("select");
		}
		else if (ready)
			ping_recv(ping);
		else
		{
			ping_send(ping);
			last = get_current_time();
		}
	}
	print_footer(*ping);
}

static void	_handle_args(t_args *args, int ac, char **av)
{
	if (ac <= 1)
		error_exit_usage("missing host operand");
	parse_args(args, ac, av);
	if (args->flags[HELP])
	{
		print_help();
		exit(EXIT_SUCCESS);
	}
	if (!args->params[0])
		error_exit_usage("missing host operand");
}

int	main(int ac, char **av)
{
	t_args	args = {0};
	t_ping	ping = {0};

	_handle_args(&args, ac, av);
	ping_init(&ping, &args);
	signal(SIGINT, &sig_int);
	_ping_run(&ping);
	return (0);
}
