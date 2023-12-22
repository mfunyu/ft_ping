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

int	get_value(char *value)
{
	int	error;
	int	ret;

	ret = ft_atoi_check(value, &error);
	if (error == ERROR)
		error_exit("invalid value");
	return (ret);
}

int	parse_value(char *option, char **av, bool is_short) // no value, pass current option and av
{
	// check case of '-c123'
	if (is_short)
	{
		if (option[1])
			return (get_value(option[1]));
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
		error_exit_value("option '' requires an argument");
}

int	match_long_option(char *option, t_args *args)
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

int		match_short_option(char *option)
{
	for (int i = 0; g_options[i].flag; i++)
	{
		if (option[0] != g_options[i].short_option)
			return (i);
	}
	error_exit_value("invalid option --", &option);
	return (0);
}

void	parse_option(char **av, t_args *args)
{
	t_flags	match;
	char	*option;
	int		idx;
	t_flags	flag;

	option = av[0];
	if (option[1] == '-')
	{
		idx = match_long_option(option, args);
		flag = get_options[idx].flag;
		if (g_options[idx].req_value)
			args->flags[flag] = parse_value(option, av, false);
		else if (ft_strchr(option, '='))
			error_exit_usage("ping: option '--numeric' doesn't allow an argument");
		else
			args->flags[flag] = 1;
		return ;
	}
	for (int j = 1; j < (int)ft_strlen(option); j++)
	{
		idx = match_short_option(option + j, args);
		flag = get_options[idx].flag;
		if (g_options[idx].req_value)
			args->flags[flag] = parse_value(option + j, av, true);
		else
			args->flags[flag] = 1;
	}
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
