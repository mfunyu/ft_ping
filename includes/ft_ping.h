#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <stddef.h>
# include <limits.h>

# define ICMP_DEFAULT_DATA_SIZE		64 - sizeof(struct icmphdr)
# define ICMP_MAX_PACKET_SIZE		65400

/* ip header */
# define FRAGMENT_OFFSET_BITS		13
# define FRAGMENT_OFFSET_MASK		((1 << FRAGMENT_OFFSET_BITS) - 1)
# define FRAGMENT_OFFSET(frag_off)	((frag_off) & FRAGMENT_OFFSET_MASK)
# define IPHDR_ALIGNMENT_BITS		2

# define PING_DEFAULT_INTERVAL_S	1 /* second */

typedef struct s_args	t_args;
typedef struct s_ping	t_ping;

void	parse_args(t_args *args, int ac, char **av);

void	ping_init(t_ping *ping, t_args *args);
void	ping_send(t_ping *ping);
void	ping_recv(t_ping *ping);

uint16_t	icmp_calc_checksum(char *msg, size_t len);
void		icmp_set_icmphdr(char *msg, int ident, int seqno);
void		icmp_add_timestamp(char *msg);
void		icmp_set_data(char *msg, size_t total_len);
void		icmp_add_checksum(char *msg, size_t len);

#endif /* FT_PING_H */
