#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <bits/types/struct_timeval.h>

int	gettimeofday(struct timeval *tv, struct timezone *tz)
{
	static int				(*orig_gettimeofday)(struct timeval *, struct timezone *);
	static struct timeval	tv_start;
	int						ret;

	if (!orig_gettimeofday)
	{
		orig_gettimeofday = dlsym(RTLD_NEXT, "gettimeofday");
		printf("--- gettimeofday: time will be modified ---\n");
		orig_gettimeofday(&tv_start, tz);
	}
	// tv->tv_sec = tv_start.tv_sec;
	// tv->tv_usec = tv_start.tv_usec;
	ret = orig_gettimeofday(tv, tz);
	tv->tv_sec = tv->tv_sec - tv_start.tv_sec;
	tv->tv_usec = tv->tv_usec - tv_start.tv_usec;
	while (tv->tv_usec < 0)
	{
		tv->tv_sec--;
		tv->tv_usec += 1000000;
	}
	return (ret);
}