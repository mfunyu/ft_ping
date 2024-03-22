NAME	:= ft_ping

SRCS	:= ft_ping.c \
			icmp.c \
			parser.c \
			ping_error.c \
			ping_init.c \
			ping_recv.c \
			ping_send.c \
			utils_time.c \
			utils_math.c \
			utils_inet.c \
			print_reply.c \
			print_data.c

# ---------------------------------------------------------------------------- #
#                                     PATHS                                    #
# ---------------------------------------------------------------------------- #

DIR_INCLUDES:= includes
DIR_OBJS:= objs
VPATH	:= srcs

# ---------------------------------------------------------------------------- #
#                                   VARIABLES                                  #
# ---------------------------------------------------------------------------- #

OBJS	= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
DEPS	:= $(OBJS:.o=.d)
CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror
INCLUDES:= -I $(DIR_INCLUDES)

BONUS=1

ifdef BONUS
	CFLAGS	+= -D BONUS
endif

# ---------------------------------------------------------------------------- #
#                                     RULES                                    #
# ---------------------------------------------------------------------------- #

.PHONY: all
all		: $(NAME)

-include $(DEPS)

$(NAME)	: $(DIR_OBJS) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(DIR_OBJS)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP $(INCLUDES) -o $@ -c $<

$(DIR_OBJS):
	@mkdir $@

.PHONY: clean
clean	:
	$(RM) -R $(DIR_OBJS)

.PHONY: fclean
fclean	: clean
	$(RM) $(NAME)

.PHONY: re
re		: fclean
	$(MAKE) all

.PHONY	: bonus
bonus	:
	make BONUS=1

# ---------------------------------------------------------------------------- #
#                                    DOCKER                                    #
# ---------------------------------------------------------------------------- #

.PHONY:	build
build	:
	docker build . -t ft_ping

.PHONY:	exec
exec		:
	docker run -it ft_ping /bin/bash

# ---------------------------------------------------------------------------- #
#                                     HELP                                     #
# ---------------------------------------------------------------------------- #