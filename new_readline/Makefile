NAME = new_readline.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = \
	history_management.c\
	history_navigation.c\
	new_readline.c\
	terminal_utils.c

OBJS = $(SRCS:.c=.o)

LIBS = -lutil -ltermcap

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
