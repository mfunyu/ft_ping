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

void	parse_value(t_args *args, int value_flag, char *value)
{
	int	error;

	printf("%s\n", value);
	args->flags[value_flag] = ft_atoi_check(value, &error);
	if (error == ERROR)
		error_exit("invalid value");
}

void	match_long_option(char *option, t_args *args)
{
	int	diff;

	for (int i = 0; options[i].flag; i++)
	{
		diff = ft_strncmp(option, options[i].long_option, ft_strlen(options[i].long_option));
		if (!diff)
		{
			args->flags[options[i].flag] = 1;
			return ;
		}
	}
	error_exit_usage("unrecognized option ''");
}

t_flags	match_short_option(char *option, t_args *args)
{
	t_flags		value;
	t_options	current;

	value = NONE;
	for (int i = 1; i < (int)ft_strlen(option); i++)
	{
		if (value)
		{
			parse_value(args, value, option + i);
			return (NONE);
		}
		for (int j = 0;; j++)
		{
			current = options[j];
			if (!current.flag)
				error_exit_usage("invalid option -- ");
			if (option[i] == current.short_option)
			{
				args->flags[current.flag] = 1;
				if (current.req_value)
					value = current.flag;
				break ;
			}
		}
	}
	return (value);
}

t_flags	match_options(char *option, t_args *args)
{

	if (option[1] == '-')
	{
		match_long_option(option, args);
		return (NONE);
	}
	return (match_short_option(option, args));
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
		if (av[i][0] == '-' && av[i][1])
		{
			value_flag = match_options(av[i], &args);
		}
		else if (value_flag)
		{
			parse_value(&args, value_flag, av[i]);
			value_flag = 0;
		}
		else
			args.params[idx++] = av[i];
	}
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
