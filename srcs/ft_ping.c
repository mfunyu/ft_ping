#include "ft_ping.h"
#include <stdlib.h>
#include <stdio.h>

void	error_exit(char *msg)
{
	printf("ft_ping: %s\n", msg);
	exit(EXIT_FAILURE);
}

int	main(int ac, char **av)
{
	if (ac <= 1)
		error_exit("usage error: Destination address required");
	(void)av;
	return (0);
}
