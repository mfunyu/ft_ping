#ifndef UTLIS_H
# define UTLIS_H

struct timeval	get_current_time();
struct timeval	calc_time_sub(struct timeval tv1, struct timeval tv2);
double	calc_time_diff(struct timeval tv1, struct timeval tv2);
double	calc_sqrt(double x, double precision);

#endif /* UTILS_H */