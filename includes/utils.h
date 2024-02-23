#ifndef UTLIS_H
# define UTLIS_H

struct timeval	get_current_timestamp(void);
size_t			calc_timetrip(struct timeval *tv_start, struct timeval *tv_end);

#endif /* UTILS_H */