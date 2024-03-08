#include <sys/types.h>
#include <stdio.h>

pid_t	getpid(void)
{
	printf("--- PID replaced with 4242 ---\n");
	return 4242;
}