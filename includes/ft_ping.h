#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"
# include <stdint.h>
# include <stddef.h>
# include <netinet/ip_icmp.h>

# define ICMP_DEFAULT_PACKET_SIZE	64
# define ICMP_MAX_PACKET_SIZE		65400

/*
**  0                   1                   2                   3
**     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |     Type      |     Code      |          Checksum             |
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |           Identifier          |        Sequence Number        |
**    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
**    |     Data ...
*/

typedef struct	s_icmp_send
{
	int				len;
	struct addrinfo	*addr;
	int				seq;
	struct timeval	tv;
	char			msg[ICMP_MAX_PACKET_SIZE];
	char			ip[INET_ADDRSTRLEN];
}				t_icmp_send;

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);
void	init_send(t_icmp_send *send, t_args *args);

void	handle_send(int sfd, t_icmp_send *send);
void	handle_recv(int sfd, t_icmp_send *send);

void	icmp_echo_request_message(char *msg, size_t len);

struct addrinfo	*host_to_addrinfo(char const *hostname);
int				create_raw_socket(void);
void			cleanup(struct addrinfo *addr, int sfd);

#endif /* FT_PING_H */
