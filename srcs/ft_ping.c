#include "ft_ping.h"
#include <stdlib.h>
#include <stdio.h>

void	error_exit(char *msg)
{
	printf("ft_ping: %s\n", msg);
	exit(EXIT_FAILURE);
}

void	parse_args(int ac, char **av)
{
	char	*arg;

	for (int i = 1; i < ac; i++)
	{
		arg = av[i];
		printf("%s\n", arg);
		if (arg[0] == '-')
		{

		}
		else
		{
			
		}
	}
}

int	main(int ac, char **av)
{
	if (ac <= 1)
		error_exit("usage error: Destination address required");
	parse_args(ac, av);
	return (0);
}
