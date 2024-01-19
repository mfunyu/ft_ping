#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);

struct addrinfo	*host_to_addrinfo(char const *hostname);
int				create_raw_socket(void);
void			send_packet(struct addrinfo *addr, int sfd);
void			cleanup(struct addrinfo *addr, int sfd);

#endif /* FT_PING_H */
