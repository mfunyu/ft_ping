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

int	parse_value(char *value) // no value, pass current option and av
{
	// check case of '-c123'
	// check case of '--c=123'
	// check next of av
	// if non match, ping: option requires an argument -- 'c'
	// or ping: option '--count' requires an argument
	int	error;
	int	ret;

	ret = ft_atoi_check(value, &error);
	if (error == ERROR)
		error_exit("invalid value");
	return (ret);
}

void	match_long_option(char **av, t_args *args)
{
	int		c;
	char	*option;

	option = av[0];
	for (int i = 0; g_options[i].flag; i++)
	{
		c = ft_strlen("--");
		while (option[c] && g_options[i].long_option[c] == option[c])
			c++;
		if (!option[c])
		{
			args->flags[g_options[i].flag] = 1;
			if (g_options[i].req_value)
				args->flags[g_options[i].flag] = parse_value(option + 1);
			return ;
		}
		if (option[c] == '=')
		{
			if (!g_options[i].req_value)
				error_exit_usage("ping: option '--numeric' doesn't allow an argument");
			args->flags[g_options[i].flag] = parse_value(option + c + 1);
			return ;
		}
	}
	error_exit_value("unrecognized option", option);
}

t_flags	match_short_option(char option)
{
	for (int i = 0; g_options[i].flag; i++)
	{
		if (option == g_options[i].short_option)
			return (g_options[i].flag);
	}
	error_exit_value("invalid option --", &option);
	return (NONE);
}

t_flags	parse_option(char **av, t_args *args)
{
	t_flags	match;
	t_flags	flag;
	char	*option;

	option = av[0];
	if (option[1] == '-')
	{
		match_long_option(av, args);
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
		if (g_options[match - 1].req_value)
			flag = match;
	}
	if (flag)
		error_exit_value("option requires an argument --",
			&g_options[flag - 1].short_option);
	return (flag);
}

t_args	parse_args(int ac, char **av)
{
	t_args		args = {0};
	int			idx;

	args.params = av;
	idx = 0;
	for (int i = 1; i < ac; i++)
	{
		if (args.flags[HELP])
			return (args);
		if (av[i][0] == '-' && av[i][1])
			parse_option(av + i, &args);
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
