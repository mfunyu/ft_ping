#include <stdio.h>
#include "ft_ping.h"
#include "ping_struct.h"
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
	printf("PING %s (%s): %d data bytes", ping.dst_hostname, ping.dst_ip, ping.datalen);
	if (ping.verbose)
		printf(", id 0x%04x = %d", ping.ident, ping.ident);
	printf("\n");
}

static void	_print_stats(t_stat stats, size_t num_recv)
{
	double	avg;
	double	stddev;

	avg = calc_avg(stats.sum, num_recv);
	stddev = calc_stddev(stats.sum, stats.sum_sq, num_recv);
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
		stats.min, avg, stats.max, stddev);
}

void	print_footer(t_ping ping)
{
	printf("--- %s ping statistics ---\n", ping.dst_hostname);
	printf("%zu packets transmitted, %zu packets received, %zu%% packet loss\n",
		ping.num_xmit, ping.num_recv, (ping.num_xmit - ping.num_recv) * 100 / ping.num_xmit);

	if (ping.num_recv)
		_print_stats(ping.stats, ping.num_recv);
}
