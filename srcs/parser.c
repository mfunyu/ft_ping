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

bool	parse_long_option(char **av, t_args *args)
{
	char	*option;
	int		idx;
	t_flags	flag;

	option = av[0];
	idx = match_long_option(option);
	flag = g_options[idx].flag;
	if (ft_strchr(option, '='))
	{
		if (!g_options[idx].req_value)
			error_exit_usage("ping: option '--xxx' doesn't allow an argument");
		args->flags[flag] = parse_value(option + ft_strlen("--"));
		return (false);
	}
	args->flags[flag] = 1;
	if (g_options[idx].req_value)
	{
		if (!av[1])
			error_exit_value("option requires an argument --", g_options[idx].long_option);
		args->flags[flag] = parse_value(av[1]);
		return (true);
	}
	return (false);
}

int		match_short_option(char *option)
{
	for (int i = 0; g_options[i].flag; i++)
	{
		if (option[0] == g_options[i].short_option)
			return (i);
	}
	error_exit_value("invalid option --", option);
	return (0);
}

bool	parse_short_option(char **av, t_args *args)
{
	char	*option;
	int		idx;
	t_flags	flag;

	option = av[0];
	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		idx = match_short_option(option + j);
		flag = g_options[idx].flag;
		args->flags[flag] = 1;
		if (g_options[idx].req_value)
		{
			if (option[j + 1])
			{
				args->flags[flag] = parse_value(option + j + 1);
				return (false);
			}
			if (!av[1])
				error_exit_value("option requires an argument --", &g_options[idx].short_option);
			args->flags[flag] = parse_value(av[1]);
			return (true);
		}
	}
	return (false);
}

t_args	parse_args(int ac, char **av)
{
	t_args	args = {0};
	int		idx;
	bool	next;

	args.params = av;
	idx = 0;
	next = false;
	for (int i = 1; i < ac; i++)
	{
		if (args.flags[HELP])
			return (args);
		if (av[i][0] == '-' && av[i][1])
		{
			if (av[i][1] == '-')
				next = parse_long_option(av + i, &args);
			else
				next = parse_short_option(av + i, &args);
			i += next;
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
