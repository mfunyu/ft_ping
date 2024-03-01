#include <stdio.h>
#include "ft_ping.h"
#include "utils.h"

void	print_help(void)
{
	printf("Usage: ./ft_ping [OPTION...] HOST ...\n\
Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
	printf(" Options valid:\n\
  -v, --verbose              verbose output\n");
# ifdef BONUS
	printf("\
  -c, --count=NUMBER         stop after sending NUMBER packets\n");
	printf("\
  -t, --ttl=N                specify N as time-to-live\n");
	printf("\
  -s, --size=NUMBER          ping NUMBER data octets\n");
# endif
	printf("\n\
  -?, --help                 give this help list\n\n");
	printf("Mandatory or optional arguments to long options are also mandatory or optional\n\
for any corresponding short options.\n\n");
	printf("Report bugs to: <https://github.com/mfunyu/ft_ping/issues>\n");
}

void	print_header(t_ping ping)
{
	printf("PING %s (%s): %ld data bytes\n", ping.dst_hostname, ping.dst_ip, ping.len - sizeof(struct icmphdr));
}

void	print_footer(t_ping ping)
{
	double	avg;
	double	variance;

	printf("--- %s ping statistics ---\n", ping.dst_hostname);
	printf("%zu packets transmitted, %zu packets received, %zu%% packet loss\n",
		ping.num_xmit, ping.stats.recieved, (ping.num_xmit - ping.stats.recieved) * 100 / ping.num_xmit);
	if (ping.stats.recieved == 0)
		return ;

	avg = ping.stats.sum / ping.stats.recieved;
	variance = ping.stats.sum_sq / ping.stats.recieved - avg * avg;
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		ping.stats.min, avg, ping.stats.max, calc_sqrt(variance, 0.0005));
}