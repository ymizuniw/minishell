CC = cc
CFLAGS = -g -Wall -Wextra -Werror
AR = ar rcs

SRCS = \
    ft_atoi.c \
    ft_atoll.c \
    ft_bzero.c \
    ft_calloc.c \
    ft_isalnum.c \
    ft_isalpha.c \
    ft_isascii.c \
    ft_isdigit.c \
    ft_isprint.c \
    ft_isspace.c \
    ft_itoa.c \
    ft_lst_add_back.c \
    ft_lst_last.c \
    ft_memcpy.c \
    ft_memmove.c \
    ft_memset.c \
    ft_putchar_fd.c \
    ft_putendl_fd.c \
    ft_putnbr_fd.c \
    ft_putstr_fd.c \
    ft_realloc.c \
    ft_split.c \
    ft_strchr.c \
    ft_strcmp.c \
    ft_strdup.c \
    ft_striteri.c \
    ft_strjoin.c \
    ft_strlcat.c \
    ft_strlcpy.c \
    ft_strlen.c \
    ft_strmapi.c \
    ft_strncmp.c \
    ft_strncpy.c \
    ft_strndup.c \
    ft_strnstr.c \
    ft_strrchr.c \
    ft_strstr.c \
    ft_strtrim.c \
    ft_substr.c \
    ft_tolower.c \
    ft_toupper.c \
    xfree.c \
    xmalloc.c \
    get_next_line/get_next_line.c

OBJS = $(SRCS:.c=.o)
NAME = libft.a

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
