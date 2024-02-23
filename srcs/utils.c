#include <stddef.h>
#include <stdlib.h>
#include <sys/time.h>
#include "error.h"
#include "ft_ping.h"

struct timeval	*get_current_timestamp(void)
{
	struct timeval	*tv;

	tv = (struct timeval *)malloc(sizeof(struct timeval));
	if (!tv)
		error_exit("malloc error");
	if (gettimeofday(tv, NULL))
		error_exit("gettimeofday error");
	return (tv);
}

static size_t	_calc_time_diff(struct timeval *tv_start, struct timeval *tv_end)
{
	size_t	triptime;

	triptime = (tv_end->tv_sec - tv_start->tv_sec) * 1000 * 1000;
	triptime += tv_end->tv_usec - tv_start->tv_usec;
	return (triptime);
}

size_t	calc_timetrip(t_ping *ping, struct timeval *reply_timestamp)
{
	size_t	triptime;
	t_list	*next;

	triptime = _calc_time_diff(ping->req_timestamps->content, reply_timestamp);
	next = ping->req_timestamps->next;
	ft_lstdelone(ping->req_timestamps, free);
	ping->req_timestamps = next;
	return (triptime);
}