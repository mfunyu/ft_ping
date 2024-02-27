#ifndef UTLIS_H
# define UTLIS_H

void	get_current_timestamp(struct timeval *tv);
struct timeval	calc_time_sub(struct timeval tv1, struct timeval tv2);
double	calc_time_diff(struct timeval tv1, struct timeval tv2);
double	calc_sqrt(double x, double precision);

#endif /* UTILS_H */