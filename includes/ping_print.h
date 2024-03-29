#ifndef PING_PRINT_H
# define PING_PRINT_H

/* print_data.c */
void	print_help(void);
void	print_header(t_ping ping);
void	print_footer(t_ping ping);

/* print_reply.c */
void	print_reply(t_echo_data *echo_data, bool verbose, bool is_dup);

#endif /* PING_PRINT_H */