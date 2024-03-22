#include <stddef.h>
#include <limits.h>
#include <stdbool.h>
#include <ctype.h>

double	calc_square(double x)
{
	return (x * x);
}

double	calc_sqrt(double x, double precision)
{
	double 	root;

	if (x < precision)
		return (0);
	root = x / 2;
	while (calc_square(root) - x > precision)
		root = (root + x / root) / 2;
	return (root);
}

double	calc_avg(double sum, size_t n)
{
	return (sum / n);
}

double	calc_stddev(double sum, double sum_sq, size_t n)
{
	double	avg;
	double	variance;

	avg = calc_avg(sum, n);
	variance = sum_sq / n - calc_square(avg);
	return (calc_sqrt(variance, 0.0005));
}

static bool	is_space(const char c)
{
	return (c == ' ' || c == '\n' || c == '\t'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	ft_atoi_check(const char *n, int *error)
{
	long long	nb;
	int			sign;

	*error = true;
	while (is_space(*n))
		n++;
	sign = 1;
	if (*n == '-' || *n == '+')
		sign *= 44 - *n++;
	nb = 0;
	while (isdigit(*n))
	{
		*error = false;
		nb = nb * 10 + (*n++ - '0');
		if (nb * sign < INT_MIN || INT_MAX < nb * sign)
		{
			*error = true;
			return (0);
		}
	}
	if (*n != '\0')
		*error = true;
	return ((int)(nb * sign));
}