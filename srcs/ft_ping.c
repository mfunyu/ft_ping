#include "ft_ping.h"
#include "error.h"
#include <stdio.h>

void	help(void)
{
	printf("Usage: ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\n\
 Options valid for all request types:\n\
  -v, --verbose              verbose output\n\n\
  -?, --help                 give this help list\n\n");
}

int	main(int ac, char **av)
{
	t_args	args;

	if (ac <= 1)
		error_exit_usage("missing host operand");
	args = parse_args(ac, av);
	if (args.flags[HELP])
	{
		help();
		return (0);
	}
	if (!args.params[0])
		error_exit_usage("missing host operand");
	print_args(args);
	return (0);
}
