#ifndef UTLIS_H
# define UTLIS_H

struct timeval	get_current_timestamp(void);
double			calc_timetrip(struct timeval *tv_start, struct timeval *tv_end);
double			calc_sqrt(double x, double precision);

#endif /* UTILS_H */