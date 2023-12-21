#ifndef FT_PING_H
# define FT_PING_H

# include <stdbool.h>

typedef enum	e_flags
{
	NONE = 0,
	HELP,
	VERBOSE
}				t_flags;

typedef struct	s_options
{
	char	*short_option;
	char	*long_option;
	t_flags	flag;
	bool	req_value;
}				t_options;

#endif /* FT_PING_H */
