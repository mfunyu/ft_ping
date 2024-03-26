#include <stddef.h>
#include <sys/time.h>
#include "ping_error.h"

struct timeval	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit_strerr("gettimeofday error");
	return (tv);
}

void	normalize_time(struct timeval *tv)
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

struct timeval	sub_time(struct timeval tv1, struct timeval tv2)
{
	struct timeval	sub = {
		.tv_sec = tv2.tv_sec - tv1.tv_sec,
		.tv_usec = tv2.tv_usec - tv1.tv_usec,
	};
	normalize_time(&sub);
	return (sub);
}

double	diff_time(struct timeval tv1, struct timeval tv2)
{
	double	diff;

	diff = (tv2.tv_sec - tv1.tv_sec) * 1000;
	diff += ((double)tv2.tv_usec - (double)tv1.tv_usec) / 1000;
	return (diff);
}

struct timeval	get_timeout_time(struct timeval interval, struct timeval last)
{
	struct timeval	now;
	struct timeval	passed;
	struct timeval	rest;

	now = get_current_time();
	passed = sub_time(last, now);
	rest = sub_time(passed, interval);
	return (rest);
}
