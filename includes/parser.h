#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>

typedef enum	e_flags
{
	NONE = 0,
	HELP,
	VERBOSE,
# ifdef BONUS
	COUNT,
# endif
	TOTAL
}				t_flags;

typedef enum	e_parse_errs
{
	INVALID,
	MISSING,
	NOT_ALLOWED,
}				t_parse_errs;

typedef struct	s_options
{
	char	short_option;
	char	*long_option;
	t_flags	flag;
	bool	req_value;
}				t_options;

typedef struct	s_args
{
	int		flags[TOTAL];
	char	**params;
}				t_args;

#endif /* PARSER_H */
