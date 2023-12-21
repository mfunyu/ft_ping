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

t_flags	parse_option(char *option, t_args *args)
{
	t_flags	match;
	t_flags	flag;

	if (option[1] == '-')
	{
		match = match_long_option(option);
		args->flags[match] = 1;
		return (NONE);
	}
	flag = NONE;
	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		if (flag)
		{
			args->flags[flag] = parse_value(option + j);
			return (NONE);
		}
		match = match_short_option(option[j]);
		args->flags[match] = 1;
		if (options[match - 1].req_value)
			flag = match;
	}
	return (flag);
}

t_args	parse_args(int ac, char **av)
{
	t_args		args = {0};
	t_flags		flag;
	int			idx;

	args.params = av;
	flag = NONE;
	idx = 0;
	for (int i = 1; i < ac; i++)
	{
		if (flag)
		{
			args.flags[flag] = parse_value(av[i]);
			flag = NONE;
		}
		else if (av[i][0] == '-' && av[i][1])
			flag = parse_option(av[i], &args);
		else
			args.params[idx++] = av[i];
	}
	if (flag)
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
