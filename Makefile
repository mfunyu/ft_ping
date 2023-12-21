NAME	:= ft_ping

SRCS	:= ft_ping.c

# ---------------------------------------------------------------------------- #
#                                     PATHS                                    #
# ---------------------------------------------------------------------------- #

DIR_INCLUDES:= includes
DIR_OBJS:= objs
LIBFT	:= libft
VPATH	:= srcs

# ---------------------------------------------------------------------------- #
#                                   VARIABLES                                  #
# ---------------------------------------------------------------------------- #

OBJS	= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
DEPS	:= $(OBJS:.o=.d)
CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
INCLUDES:= -I $(DIR_INCLUDES) -I $(LIBFT)

# ---------------------------------------------------------------------------- #
#                                     RULES                                    #
# ---------------------------------------------------------------------------- #

.PHONY: all
all		: $(NAME)

-include $(DEPS)

$(NAME)	: $(DIR_OBJS) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT) -lft

$(DIR_OBJS)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP $(INCLUDES) -o $@ -c $<

$(DIR_OBJS):
	@mkdir $@

.PHONY: clean
clean	:
	$(MAKE) clean -C $(LIBFT)
	$(RM) -R $(DIR_OBJS)

.PHONY: fclean
fclean	: clean
	$(MAKE) fclean -C $(LIBFT)
	$(RM) $(NAME)

.PHONY: re
re		: fclean all

# ---------------------------------------------------------------------------- #
#                                    DOCKER                                    #
# ---------------------------------------------------------------------------- #

.PHONY:	build
build	:
	docker build . -t ft_ping

.PHONY:	exec
exec		:
	docker run -it ft_ping /bin/bash
