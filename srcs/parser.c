#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

static t_options	options[] = {
	{'?', "--help",		HELP,	false},
	{'v', "--verbose",	VERBOSE,false},
	{'\0', "\0",		NONE,	false}
};

t_options	match_options(char *option, t_args *args)
{
	int			diff;

	if (option[1] == '-')
	{
		for (int i = 0; options[i].flag; i++)
		{
			diff = ft_strncmp(option, options[i].long_option, ft_strlen(options[i].long_option));
			if (!diff)
			{
				args->flags[options[i].flag] = 1;
				return (options[i]);
			}
		}
	}
	else
	{
		for (int i = 1; i < (int)ft_strlen(option); i++)
		{
			for (int j = 0; options[j].flag; j++)
			{
				if (option[i] == options[j].short_option)
				{
					args->flags[options[i].flag] = 1;
					return (options[j]);
				}
			}
		}
	}
	return (options[2]);
}

t_args	parse_args(int ac, char **av)
{
	t_args		args = {0};
	int			value_flag;
	int			idx;
	t_options	opt;
	int			error;

	args.params = av;
	value_flag = 0;
	idx = 0;
	for (int i = 1; i < ac; i++)
	{
		if (av[i][0] == '-')
		{
			opt = match_options(av[i], &args);
			if (opt.req_value)
				value_flag = opt.flag;
		}
		else if (value_flag)
		{
			args.flags[value_flag] = ft_atoi_check(av[i], &error);
			if (error == ERROR)
			{
				printf("ping: invalid value");
				exit(1);
			}
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
