#ifndef PING_ERROR_H
# define PING_ERROR_H

void	error_exit_usage(char *msg);
void	error_exit(char *msg);
void	error_exit_gai(char *msg, int ret);

#endif /* PING_ERROR_H */
