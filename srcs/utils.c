#include <stddef.h>
#include <sys/time.h>
#include "error.h"

void	get_current_timestamp(struct timeval *tv)
{
	if (gettimeofday(tv, NULL))
		error_exit("gettimeofday error");
}

void	calc_time_normalize(struct timeval *tv)
{
	while (tv->tv_usec < 0)
	{
		tv->tv_sec--;
		tv->tv_usec += 1000000;
	}
	while (tv->tv_usec >= 1000000)
	{
		tv->tv_sec++;
		tv->tv_usec -= 1000000;
	}
	if (tv->tv_sec < 0)
	{
		tv->tv_sec = 0;
		tv->tv_usec = 0;
	}
}

struct timeval	calc_time_sub(struct timeval tv1, struct timeval tv2)
{
	struct timeval	sub = {
		.tv_sec = tv2.tv_sec - tv1.tv_sec,
		.tv_usec = tv2.tv_usec - tv1.tv_usec,
	};
	calc_time_normalize(&sub);
	return (sub);
}

double	calc_time_diff(struct timeval tv1, struct timeval tv2)
{
	double	diff;

	diff = (tv2.tv_sec - tv1.tv_sec) * 1000;
	diff += ((double)tv2.tv_usec - (double)tv1.tv_usec) / 1000;
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
