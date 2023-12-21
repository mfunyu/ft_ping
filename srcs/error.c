#include <stdio.h>
#include <stdlib.h>

void	error_exit_value(char *msg, char *value)
{
	fprintf(stderr, "ft_ping: %s '%s'\n", msg, value);
	fprintf(stderr, "Try './ft_ping --help' for more information.\n");
	exit(EXIT_FAILURE);
}

void	error_exit_usage(char *msg)
{
	fprintf(stderr, "ft_ping: %s\n", msg);
	fprintf(stderr, "Try './ft_ping --help' for more information.\n");
	exit(EXIT_FAILURE);
}

void	error_exit(char *msg)
{
	fprintf(stderr, "ft_ping: %s\n", msg);
	exit(EXIT_FAILURE);
}
