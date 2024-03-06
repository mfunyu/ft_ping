#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <stddef.h>
# include <limits.h>

# define MAXIPLEN	60
# define MAXICMPLEN	76
# define PING_DEFAULT_DATALEN		64 - sizeof(struct icmphdr)
# define PING_MAX_DATALEN			65535 - MAXIPLEN - MAXICMPLEN
# define PING_DEFAULT_INTERVAL_S	1 /* second */

/* ip header */
# define FRAGMENT_OFFSET_BITS		13
# define FRAGMENT_OFFSET_MASK		((1 << FRAGMENT_OFFSET_BITS) - 1)
# define FRAGMENT_OFFSET(frag_off)	((frag_off) & FRAGMENT_OFFSET_MASK)
# define IPHDR_ALIGNMENT_BITS		2

typedef struct s_args	t_args;
typedef struct s_ping	t_ping;

void	parse_args(t_args *args, int ac, char **av);

void	ping_init(t_ping *ping, t_args *args);
void	ping_send(t_ping *ping);
void	ping_recv(t_ping *ping);

#endif /* FT_PING_H */
