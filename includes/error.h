#ifndef ERROR_H
# define ERROR_H

void	error_exit_usage(char *msg);
void	error_exit(char *msg);
void	error_exit_gai(char *msg, int ret);

#endif /* ERROR_H */
