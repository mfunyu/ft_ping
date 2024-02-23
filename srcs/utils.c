#include <stddef.h>
#include <sys/time.h>
#include "error.h"

struct timeval	get_current_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("gettimeofday error");
	return (tv);
}

double	calc_timetrip(struct timeval *tv_start, struct timeval *tv_end)
{
	double	triptime;

	triptime = (tv_end->tv_sec - tv_start->tv_sec) * 1000;
	triptime += ((double)tv_end->tv_usec - (double)tv_start->tv_usec) / 1000;
	return (triptime);
}

double	calc_sqrt(double x, double precision)
{
	double 	root;

	if (x < precision)
		return (0);
	root = x / 2;
	while (root * root - x > precision)
		root = (root + x / root) / 2;
	return (root);
}
