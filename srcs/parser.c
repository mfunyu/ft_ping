#include "parser.h"
#include "error.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

/* Must follows the exact order of enum e_flags */
static t_options	g_options[] = {
{'?', "--help",		HELP,		false},
{'v', "--verbose",	VERBOSE,	false},
# ifdef BONUS
{'c', "--count",	COUNT,		true},
# endif
{'\0', "\0",		NONE,		false}
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

int	parse_value2(char *option, char **av, bool is_short) // no value, pass current option and av
{
	(void)option;
	(void)av;
	(void)is_short;
	/*
	// check case of '-c123'
	if (is_short)
	{
		if (option[1])
			return (parse_value(option[1]));
	}
	// check case of '--c=123'
	else
	{
		argument = ft_strchr(option, '=');
		if (argument)
			return (get_value(argument));
	}
	// check next of av
	if (av + 1)
		return (get_value(av + 1));

	if (is_short)
		error_exit_value("option requires an argument --", option);
	else
		error_exit_value("option '' requires an argument");*/
		return (0);
}

int	match_long_option(char *option)
{
	int		c;

	for (int i = 0; g_options[i].flag; i++)
	{
		c = ft_strlen("--");
		while (option[c] && g_options[i].long_option[c] == option[c])
			c++;
		if (!option[c] || option[c] == '=')
			return (i);
	}
	error_exit_value("unrecognized option", option);
	return (0);
}

t_flags	parse_long_option(char *option, t_args *args)
{
	int		idx;
	t_flags	flag;

	idx = match_long_option(option);
	flag = g_options[idx].flag;
	if (ft_strchr(option, '='))
	{
		if (!g_options[idx].req_value)
			error_exit_usage("ping: option '--xxx' doesn't allow an argument");
		args->flags[flag] = parse_value(option + ft_strlen("--"));
		return (NONE);
	}
	args->flags[flag] = 1;
	if (g_options[idx].req_value)
		return (flag);
	return (NONE);
}

int		match_short_option(char *option)
{
	for (int i = 0; g_options[i].flag; i++)
	{
		if (option[0] != g_options[i].short_option)
			return (i);
	}
	error_exit_value("invalid option --", option);
	return (0);
}

t_flags	parse_short_option(char *option, t_args *args)
{ 
	int		idx;
	t_flags	flag;

	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		idx = match_short_option(option + j);
		flag = g_options[idx].flag;
		args->flags[flag] = 1;
		if (g_options[idx].req_value)
		{
			if (!option[j + 1])
				return (flag);
			args->flags[flag] = parse_value(option + j + 1);
			break ;
		}
	}
	return (NONE);
}

t_args	parse_args(int ac, char **av)
{
	t_args		args = {0};
	int			idx;
	t_flags		flag;

	args.params = av;
	idx = 0;
	flag = 0;
	for (int i = 1; i < ac; i++)
	{
		if (args.flags[HELP])
			return (args);
		if (flag)
		{
			args.flags[flag] = parse_value(av[i]);
			flag = 0;
		}
		else if (av[i][0] == '-' && av[i][1])
		{
			if (av[i][1] == '-')
				flag = parse_long_option(av[i], &args);
			else
				flag = parse_short_option(av[i], &args);
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
