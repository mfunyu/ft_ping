#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void	error_exit_usage(char *msg)
{
	if (msg)
		fprintf(stderr, "ft_ping: %s\n", msg);
	fprintf(stderr, "Try './ft_ping --help' for more information.\n");
	exit(EXIT_FAILURE);
}

void	error_exit(char *msg)
{
	fprintf(stderr, "ft_ping: %s", msg);
	if (errno)
		fprintf(stderr, " - %s", strerror(errno));
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
