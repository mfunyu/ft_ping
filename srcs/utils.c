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

size_t	calc_timetrip(struct timeval *tv_start, struct timeval *tv_end)
{
	size_t	triptime;

	triptime = (tv_end->tv_sec - tv_start->tv_sec) * 1000 * 1000;
	triptime += tv_end->tv_usec - tv_start->tv_usec;
	return (triptime);
}