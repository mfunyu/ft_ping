#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h>

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

void	wait_max_one_sec(struct timeval *tv)
{
	struct timeval	now;
	ssize_t			timepassed;

	if (gettimeofday(&now, NULL))
		error_exit("gettimeofday");

	timepassed = now.tv_sec * 1000 * 1000 + now.tv_usec;
	timepassed -= tv->tv_sec * 1000 * 1000 + tv->tv_usec;
	if (timepassed >= 1000 * 1000)
		return;
	printf("%zu\n", 1000 * 1000 - timepassed);
	usleep(1000 * 1000 - timepassed);
}

void	ft_ping(t_args *args)
{
	int				sfd;
	t_icmp_send		send;
	struct timeval	tv;

	sfd = create_raw_socket();

	init_send(&send, args);
	init_recv(sfd);
	printf("PING %s (%s) %ld data bytes\n", args->params[0], send.ip, send.len - sizeof(struct icmphdr));
	for(;;){
		if (gettimeofday(&tv, NULL))
			error_exit("gettimeofday");

		handle_send(sfd, &send);

		handle_recv(sfd, &send);

		wait_max_one_sec(&tv);
	}
	cleanup(send.addr, sfd);
}

int	main(int ac, char **av)
{
	t_args	args = {0};

	if (ac <= 1)
		error_exit_usage("missing host operand");
	parse_args(&args, ac, av);
	if (args.flags[HELP])
	{
		help();
		return (0);
	}
	if (!args.params[0])
		error_exit_usage("missing host operand");
	print_args(args);
	ft_ping(&args);
	return (0);
}
