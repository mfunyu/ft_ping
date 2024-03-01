#ifndef UTILS_H
# define UTILS_H

/* utils_time.c */
struct timeval	get_current_time();
struct timeval	get_timeout_time(struct timeval interval, struct timeval last);
struct timeval	sub_time(struct timeval tv1, struct timeval tv2);
double			diff_time(struct timeval tv1, struct timeval tv2);

/* utils_math.c */
double	calc_square(double x);
double	calc_sqrt(double x, double precision);
double	calc_avg(double sum, size_t n);
double	calc_stddev(double sum, double sum_sq, size_t n);

#endif /* UTILS_H */