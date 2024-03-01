#ifndef UTILS_TIME_H
# define UTILS_TIME_H

struct timeval	get_current_time();
struct timeval	get_timeout_time(struct timeval interval, struct timeval last);
struct timeval	sub_time(struct timeval tv1, struct timeval tv2);
double	diff_time(struct timeval tv1, struct timeval tv2);
double	calc_sqrt(double x, double precision);

#endif /* UTILS_TIME_H */