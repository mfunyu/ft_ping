#include "parser.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

t_options	match_options(char *option)
{
	t_options	lst[] = {
		{"-?", "--help",	HELP,		false},
		{"-v", "--verbose", VERBOSE,	false},
		{"\0", "\0",		NONE,		false}
	};
	int			diff;

	if (option[1] == '-')
	{
		for (int i = 0; lst[i].flag; i++)
		{
			diff = ft_strncmp(option, lst[i].long_option, ft_strlen(lst[i].long_option));
			if (!diff)
				return lst[i];
		}
	}
	return lst[2];
}

void	parse_args(int ac, char **av)
{
	char	**params;
	int		idx;

	idx = 0;
	params = malloc(sizeof(char *) * ac);
	params[ac - 1] = NULL;
	for (int i = 1; i < ac; i++)
	{
		//printf("%s\n", av[i]);
		if (av[i][0] == '-')
			match_options(av[i]);
		else
			params[idx++] = av[i];
	}
}
