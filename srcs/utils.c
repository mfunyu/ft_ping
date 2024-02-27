#include <stddef.h>
#include <sys/time.h>
#include "error.h"

void	get_current_timestamp(struct timeval *tv)
{
	if (gettimeofday(tv, NULL))
		error_exit("gettimeofday error");
}

double	calc_time_diff(struct timeval *tv1, struct timeval *tv2)
{
	double	diff;

	diff = (tv2->tv_sec - tv1->tv_sec) * 1000;
	diff += ((double)tv2->tv_usec - (double)tv1->tv_usec) / 1000;
	return (diff);
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
