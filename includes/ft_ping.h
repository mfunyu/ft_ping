#ifndef FT_PING_H
# define FT_PING_H

# include "parser.h"

void	parse_args(t_args *args, int ac, char **av);
void	print_args(t_args args);

int		dns_resolution(char const *hostname);

#endif /* FT_PING_H */
