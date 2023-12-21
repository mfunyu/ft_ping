#include "ft_ping.h"
#include "error.h"

int	main(int ac, char **av)
{
	t_args	args;

	if (ac <= 1)
		error_exit_usage("missing host operand");
	args = parse_args(ac, av);
	print_args(args);
	return (0);
}
