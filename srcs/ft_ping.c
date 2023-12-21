#include "ft_ping.h"
#include <stdio.h>
#include <stdlib.h>

void	error_exit(char *msg)
{
	fprintf(stderr, "ft_ping: %s\n", msg);
	fprintf(stderr, "Try './ft_ping --help' for more information.\n");
	exit(EXIT_FAILURE);
}

int	main(int ac, char **av)
{
	if (ac <= 1)
		error_exit("missing host operand");
	parse_args(ac, av);
	return (0);
}
