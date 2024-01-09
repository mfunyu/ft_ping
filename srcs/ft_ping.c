#include "ft_ping.h"
#include "error.h"
#include <stdio.h>

void	help(void)
{
	printf("Usage: ./ft_ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf(" Options valid for all request types:\n\
  -v, --verbose              verbose output\n");
# ifdef BONUS
	printf("\
  -c, --count=NUMBER         stop after sending NUMBER packets\n");
# endif
	printf("\n\
  -?, --help                 give this help list\n\n");
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
	return (0);
}
