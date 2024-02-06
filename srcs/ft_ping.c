#include "ft_ping.h"
#include "error.h"
#include <stdio.h>

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
	struct addrinfo	*addr;
	int				sfd;
	size_t			size;
	char			msg[ICMP_MAX_PACKET_SIZE];

	addr = host_to_addrinfo(args->params[0]);
	sfd = create_raw_socket();

	size = ICMP_DEFAULT_PACKET_SIZE;
	if (args->flags[SIZE])
		size = args->flags[SIZE];
	icmp_echo_request_message(msg, size);
	send_packet(addr, sfd, msg, size);

	receive_packet(sfd);
	cleanup(addr, sfd);
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
