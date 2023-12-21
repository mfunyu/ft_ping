#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

t_options	match_options(char *option)
{
	t_options	lst[] = {
		{'?', "--help",	HELP,		false},
		{'v', "--verbose", VERBOSE,	false},
		{'\0', "\0",		NONE,		false}
	};
	int			diff;

	if (option[1] == '-')
	{
		for (int i = 0; lst[i].flag; i++)
		{
			diff = ft_strncmp(option, lst[i].long_option, ft_strlen(lst[i].long_option));
			if (!diff)
				return (lst[i]);
		}
	}
	else
	{
		for (int i = 1; i < (int)ft_strlen(option); i++)
		{
			for (int j = 0; lst[j].flag; j++)
			{
				if (option[i] == lst[j].short_option)
					return (lst[j]);
			}
		}
	}
	return (lst[2]);
}

void	parse_args(int ac, char **av)
{
	char		*param;
	bool		is_value;
	t_options	opt;

	param = NULL;
	is_value = 0;
	for (int i = 1; i < ac; i++)
	{
		printf("%s\n", av[i]);
		if (av[i][0] == '-')
		{
			opt = match_options(av[i]);
			printf("-> %c\n", opt.short_option);
			is_value = opt.flag;
		}
		else if (is_value)
		{
			//ft_atoi_check(av[i]);
		}
		else if (!param)
			param = av[i];
	}
	if (param)
		printf("%s\n", param);
}
