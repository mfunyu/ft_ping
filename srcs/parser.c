#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

t_options	match_options(char *option, t_args *args)
{
	t_options	lst[] = {
		{'?', "--help",		HELP,	false},
		{'v', "--verbose",	VERBOSE,false},
		{'\0', "\0",		NONE,	false}
	};
	int			diff;

	if (option[1] == '-')
	{
		for (int i = 0; lst[i].flag; i++)
		{
			diff = ft_strncmp(option, lst[i].long_option, ft_strlen(lst[i].long_option));
			if (!diff)
			{
				args->flags[lst[i].flag] = 1;
				return (lst[i]);
			}
		}
	}
	else
	{
		for (int i = 1; i < (int)ft_strlen(option); i++)
		{
			for (int j = 0; lst[j].flag; j++)
			{
				if (option[i] == lst[j].short_option)
				{
					args->flags[lst[i].flag] = 1;
					return (lst[j]);
				}
			}
		}
	}
	return (lst[2]);
}

void	parse_args(int ac, char **av, t_args *args)
{
	int			value_flag;
	t_options	opt;
	int			idx;
	int			error;

	value_flag = 0;
	args->params = av;
	idx = 0;
	for (int i = 1; i < ac; i++)
	{
		printf("%s\n", av[i]);
		if (av[i][0] == '-')
		{
			opt = match_options(av[i], args);
			printf("-> %c\n", opt.short_option);
			if (opt.req_value)
				value_flag = opt.flag;
		}
		else if (value_flag)
		{
			args->flags[value_flag] = ft_atoi_check(av[i], &error);
			if (error == ERROR)
			{
				printf("ping: invalid value");
				exit(1);
			}
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
