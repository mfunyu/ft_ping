#ifndef UTLIS_H
# define UTLIS_H

struct timeval	get_current_timestamp(void);
double			calc_time_diff(struct timeval *tv1, struct timeval *tv2);
double			calc_sqrt(double x, double precision);

#endif /* UTILS_H */