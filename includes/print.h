#ifndef PRINT_H
# define PRINT_H

void	print_recv(ssize_t ret, struct msghdr *msg);
void	print_stats(t_icmp_send *send, struct timeval *tv);

#endif /* PRINT_H */