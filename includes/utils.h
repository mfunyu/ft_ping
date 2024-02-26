#ifndef UTLIS_H
# define UTLIS_H

void	get_current_timestamp(struct timeval *tv);
double	calc_time_diff(struct timeval *tv1, struct timeval *tv2);
double	calc_sqrt(double x, double precision);

#endif /* UTILS_H */