#include <stddef.h>

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