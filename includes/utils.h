#ifndef UTILS_H
# define UTILS_H

# include <netinet/in.h>

/* utils_time.c */
struct timeval	get_current_time();
struct timeval	get_timeout_time(struct timeval interval, struct timeval last);
struct timeval	sub_time(struct timeval tv1, struct timeval tv2);
double			diff_time(struct timeval tv1, struct timeval tv2);

/* utils_math.c */
double			calc_square(double x);
double			calc_sqrt(double x, double precision);
double			calc_avg(double sum, size_t n);
double			calc_stddev(double sum, double sum_sq, size_t n);
int				ft_atoi_check(const char *n, int *error);

/* utils_net.c */
void			set_sockaddr_by_hostname(struct sockaddr *addr, const char *hostname);
void			set_ip_by_sockaddr(char *ip, const struct sockaddr *addr);
void			set_ip_by_in_addr(char *ip, in_addr_t saddr);
void			set_hostname_by_in_addr(char *hostname, in_addr_t saddr);

#endif /* UTILS_H */