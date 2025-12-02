# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/01 00:01:20 by ymizuniw          #+#    #+#              #
#    Updated: 2025/12/03 04:37:30 by ymizuniw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = new_readline.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR=../libft
LIBFT = $(LIBFT_DIR)/libft.a
INC = -I. -I$(LIBFT_DIR)

SRCS = \
	history_management.c\
	history_navigation.c\
	terminal_utils.c\
	history_management.c\
	history_navigation.c\
	insertion.c\
	is_del.c\
	is_esc.c\
	is_inserting.c\
	is_signal.c\
	is_cntrl.c\
	new_readline.c\
	process_del.c\
	process_down.c\
	process_esc.c\
	process_left.c\
	process_printable.c\
	process_right.c\
	process_signal.c\
	process_up.c\
	process_break_line.c\
	process_signal_branch.c\
	process_cntrl.c\
	put_char.c\
	read_char.c\
	redraw_line.c\
	resize_window.c\
	terminal_utils.c\
	write_to_buffer.c\
	new_readline.c

OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft
