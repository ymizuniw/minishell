# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/01 00:01:20 by ymizuniw          #+#    #+#              #
#    Updated: 2025/12/01 00:01:21 by ymizuniw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = new_readline.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = \
	history_management.c\
	history_navigation.c\
	new_readline.c\
	terminal_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
