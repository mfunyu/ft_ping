#include "parser.h"
#include "error.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

static t_options	options[] = {
	{'?', "--help",		HELP,	false},
	{'v', "--verbose",	VERBOSE,false},
	{'c', "--count",	COUNT,	true},
	{'\0', "\0",		NONE,	false}
};

int	parse_value(char *value)
{
	int	error;
	int	ret;

	ret = ft_atoi_check(value, &error);
	if (error == ERROR)
		error_exit("invalid value");
	return (ret);
}

t_flags	match_long_option(char *option)
{
	int		diff;
	size_t	n;

	for (int i = 0; options[i].flag; i++)
	{
		n = ft_strlen(options[i].long_option);
		diff = ft_strncmp(option, options[i].long_option, n);
		if (!diff)
			return (options[i].flag);
	}
	error_exit_usage("unrecognized option ''");
	return (NONE);
}

t_flags	match_short_option(char option)
{
	for (int i = 0; options[i].flag; i++)
	{
		if (option == options[i].short_option)
			return (options[i].flag);
	}
	error_exit_usage("invalid option -- ");
	return (NONE);
}

int	parse_option(char *option, t_args *args)
{
	t_flags	flag;
	int		value_flag;

	if (option[1] == '-')
	{
		flag = match_long_option(option);
		args->flags[flag] = 1;
		return (0);
	}
	value_flag = 0;
	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		if (value_flag)
		{
			args->flags[value_flag] = parse_value(option + j);
			return (0);
		}
		flag = match_short_option(option[j]);
		args->flags[flag] = 1;
		if (options[flag - 1].req_value)
			value_flag = flag;
	}
	return (value_flag);
}

t_args	parse_args(int ac, char **av)
{
	t_args		args = {0};
	int			value_flag;
	int			idx;

	args.params = av;
	value_flag = 0;
	idx = 0;
	for (int i = 1; i < ac; i++)
	{
		if (value_flag)
		{
			args.flags[value_flag] = parse_value(av[i]);
			value_flag = 0;
		}
		else if (av[i][0] == '-' && av[i][1])
			value_flag = parse_option(av[i], &args);
		else
			args.params[idx++] = av[i];
	}
	if (value_flag)
		error_exit_usage("option requires an argument -- 'c'");
	args.params[idx] = NULL;
	return (args);
}

void	print_args(t_args args)
{
	printf("[options]\n");
	for (int i = 0; i < TOTAL; i++)
		printf("%d: %d\n", i, args.flags[i]);

	printf("[params]\n");
	for (int i = 0; args.params[i]; i++)
		printf("%s, ", args.params[i]);
	printf("\n");
}
