#include "ft_ping.h"
#include "ping_struct.h"
#include "ping_error.h"
#include "utils.h"
#include "parser.h"
#include "ping_print.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

volatile bool	g_stop = false;

void	sig_int(int sig)
{
	(void)sig;
	g_stop = true;
}

static void	_ping_run(t_ping *ping)
{
	struct timeval	interval;
	struct timeval	last;
	int 			ready;
	int 			fd_max;
	fd_set			readfds;
	bool			finishing;

	finishing = false;
	print_header(*ping);
	ping_send(ping);
	fd_max = ping->sfd + 1;
	last = get_current_time();
	while (!g_stop)
	{
		FD_ZERO(&readfds);
		FD_SET(ping->sfd, &readfds);
		if (ping->ping_count && ping->num_recv >= ping->ping_count)
			break;
		interval = get_timeout_time(ping->interval, last);
		ready = select(fd_max, &readfds, NULL, NULL, &interval);
		if (ready < 0)
		{
			if (errno != EINTR)
				error_exit_strerr("select");
		}
		else if (ready)
			ping_recv(ping);
		else
		{
			if (!ping->ping_count || ping->num_xmit < ping->ping_count)
				ping_send(ping);
			else if (finishing)
				break;
			else
			{
				finishing = true;
				ping->interval.tv_sec = PING_MAXWAIT;
			}
			last = get_current_time();
		}
	}
	print_footer(*ping);
}

static void	_handle_args(t_args *args, int ac, char **av)
{
	if (ac <= 1)
		error_exit_usage("missing host operand");
# ifdef BONUS
	args->flags[SIZE] = -1;
# endif
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
