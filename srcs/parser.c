#include "parser.h"
#include "error.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

static t_options	g_options[] = {
{'?', "--help",		HELP,		false},
{'v', "--verbose",	VERBOSE,	false},
# ifdef BONUS
{'c', "--count",	COUNT,		true},
# endif
{'\0', "\0",		NONE,		false}
};

static void	_parse_error_exit(t_parse_errs type, char *option, bool is_short)
{
	fprintf(stderr, "ft_ping: ");
	if (type == INVALID)
	{
		if (is_short)
			fprintf(stderr, "invalid option -- '%s'", option);
		else
			fprintf(stderr, "unrecognized option '%s'", option);

	}
	else if (type == MISSING)
	{
		if (is_short)
			fprintf(stderr, "option requires an argument --%s", option);
		else
			fprintf(stderr, "option '%s' requires an argument", option);

	}
	else if (type == NOT_ALLOWED)
	{
		fprintf(stderr, "option '%s' doesn't allow an argument", option);
	}
	fprintf(stderr, "\n");
	error_exit_usage(NULL);
}

static int	_parse_value(char *value)
{
	int	error;
	int	ret;

	if (!*value)
		return (0);
	ret = ft_atoi_check(value, &error);
	if (error == ERROR)
		error_exit("invalid value");
	return (ret);
}

static int	_match_short_option(char option)
{
	for (int i = 0; g_options[i].flag; i++)
	{
		if (option == g_options[i].short_option)
			return (i);
	}
	_parse_error_exit(INVALID, &option, true);
	return (0);
}

static int	_match_long_option(char *option)
{
	int	c;

	for (int i = 0; g_options[i].flag; i++)
	{
		c = ft_strlen("--");
		while (option[c] && g_options[i].long_option[c] == option[c])
			c++;
		if (!option[c] || option[c] == '=')
			return (i);
	}
	_parse_error_exit(INVALID, option, false);
	return (0);
}

static bool	_parse_short_option(char **av, t_args *args)
{
	char	*option;
	int		idx;
	t_flags	flag;

	option = av[0];
	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		idx = _match_short_option(option[j]);
		flag = g_options[idx].flag;
		if (g_options[idx].req_value)
		{
			if (option[j + 1])
			{
				args->flags[flag] = _parse_value(option + j + 1);
				return (false);
			}
			if (!av[1])
				_parse_error_exit(MISSING, &g_options[idx].short_option, true);
			args->flags[flag] = _parse_value(av[1]);
			return (true);
		}
		args->flags[flag] = 1;
	}
	return (false);
}

static bool	_parse_long_option(char **av, t_args *args)
{
	char	*option;
	int		idx;
	t_flags	flag;

	option = av[0];
	idx = _match_long_option(option);
	flag = g_options[idx].flag;
	if (ft_strchr(option, '='))
	{
		if (!g_options[idx].req_value)
			_parse_error_exit(NOT_ALLOWED, g_options[idx].long_option, false);
		args->flags[flag] = _parse_value(ft_strchr(option, '=') + 1);
		return (false);
	}
	if (g_options[idx].req_value)
	{
		if (!av[1])
			_parse_error_exit(MISSING, g_options[idx].long_option, false);
		args->flags[flag] = _parse_value(av[1]);
		return (true);
	}
	args->flags[flag] = 1;
	return (false);
}

void	parse_args(t_args *args, int ac, char **av)
{
	int		idx;
	bool	skip;

	idx = 0;
	skip = false;
	args->params = av;
	for (int i = 1; i < ac; i++)
	{
		if (args->flags[HELP])
			return ;
		if (av[i][0] == '-' && av[i][1])
		{
			if (av[i][1] == '-')
				skip = _parse_long_option(av + i, args);
			else
				skip = _parse_short_option(av + i, args);
			i += skip;
		}
		else
			args->params[idx++] = av[i];
	}
	args->params[idx] = NULL;
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
