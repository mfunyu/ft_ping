#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>

e_status g_status = SEND;

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
  -s, --size=NUMBER          send NUMBER data octets\n");
# endif
	printf("\n\
  -?, --help                 give this help list\n\n");
	printf("Mandatory or optional arguments to long options are also mandatory or optional\n\
for any corresponding short options.\n\n");
	printf("Report bugs to: <htttps://github.com/mfunyu/ft_ping/issues>\n");
}

void	sig_alarm(int sig)
{
	(void)sig;
	g_status = SEND;
}

void	sig_int(int sig)
{
	(void)sig;
	g_status = INTERRUPT;
}

void	main_loop(int sfd, t_icmp_send *send)
{
	struct timeval	timeout = {
		.tv_sec = 0,
		.tv_usec = 10000,
	};
	int 			ready;
	fd_set			readfds;

	while (1)
	{
		if (g_status == SEND)
			handle_send(sfd, send);
		FD_ZERO(&readfds);
		FD_SET(sfd, &readfds);
		ready = select(sfd + 1, &readfds, NULL, NULL, &timeout);
		if (ready < 0)
		{
			if (errno == EINTR)
				continue;
			error_exit("select");
		}
		else if (ready)
			handle_recv(sfd, send);
	}
}

void	ft_ping(t_args *args)
{
	int				sfd;
	t_icmp_send		send;

	sfd = create_raw_socket();
	init_send(&send, args);
	init_recv(sfd);
	signal(SIGALRM, &sig_alarm);
	printf("PING %s (%s): %ld data bytes\n", args->params[0], send.ip, send.len - sizeof(struct icmphdr));
	main_loop(sfd, &send);
	cleanup(send.addr, sfd);
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
