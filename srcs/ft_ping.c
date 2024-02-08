#include "ft_ping.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>

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

void	ft_ping(t_args *args)
{
	int				sfd;
	t_icmp_send		send;

	sfd = create_raw_socket();

	init_send(&send, args);
	for(;;){
		handle_send(sfd, &send);

		handle_recv(sfd);
		usleep(1000000);
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
