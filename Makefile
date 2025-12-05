# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/15 18:41:23 by kemotoha          #+#    #+#              #
#    Updated: 2025/12/05 18:55:54 by ymizuniw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -Iincludes -Inew_readline
LDFLAGS=-L$(NEW_RL_DIR) -L$(LIBFT_DIR)

# options
# -fsanitize=address
# -g

#mac book
# CFLAGS = -g -Wall -Wextra -Werror -D_DEFAULT_SOURCE
#   export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
#   export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

# DIRECTORIES
SRC_DIR = src
BUILTIN_DIR = $(SRC_DIR)/builtin
BUILTIN_UTILS_DIR = $(BUILTIN_DIR)/builtin_utils
DATA_MANAGEMENT_DIR = $(SRC_DIR)/data_management
ENV_MANAGEMENT_DIR = $(SRC_DIR)/env_management
EXECUTER_DIR = $(SRC_DIR)/executer
EXECUTER_UTILS_DIR = $(EXECUTER_DIR)/executor_utils
WORD_EXPANSION_DIR = $(EXECUTER_UTILS_DIR)/word_expansion
AST_TRAVERSAL_DIR = $(EXECUTER_UTILS_DIR)/ast_traversal_utils
TOKEN_EXPANSION_DIR = $(EXECUTER_UTILS_DIR)/token_expansion
HEREDOC_DIR = $(EXECUTER_UTILS_DIR)/heredoc
GEN_WORD_DIR = $(EXECUTER_UTILS_DIR)/gen_word
WORD_LIST_DIR = $(EXECUTER_UTILS_DIR)/word_list
SEARCH_EXEC_DIR = $(EXECUTER_UTILS_DIR)/search_and_exec
LEXER_DIR = $(SRC_DIR)/lexer
LEXER_UTILS_DIR = $(LEXER_DIR)/lexer_utils
PARSER_DIR = $(SRC_DIR)/parser
SIGNAL_MANAGEMENT_DIR = $(SRC_DIR)/signal_management
NEW_RL_DIR = new_readline
NEW_RL_LIB = $(NEW_RL_DIR)/new_readline.a

# SOURCE FILES
MAIN_SRC = $(SRC_DIR)/main.c \
           $(SRC_DIR)/shell_init.c \
           $(SRC_DIR)/parse_exec.c \
           $(SRC_DIR)/shell_loop_utils.c

BUILTIN_SRC = $(BUILTIN_DIR)/exec_builtin.c \
              $(BUILTIN_DIR)/ft_cd/ft_cd.c \
              $(BUILTIN_DIR)/ft_cd/expand_home.c \
              $(BUILTIN_DIR)/ft_echo.c \
              $(BUILTIN_DIR)/ft_env.c \
              $(BUILTIN_DIR)/ft_exit.c \
              $(BUILTIN_DIR)/ft_export/ft_export.c \
              $(BUILTIN_DIR)/ft_export/export_print.c \
              $(BUILTIN_DIR)/ft_pwd.c \
              $(BUILTIN_DIR)/ft_unset.c

BUILTIN_UTILS_SRC = $(BUILTIN_UTILS_DIR)/get_pwd.c \
                    $(BUILTIN_UTILS_DIR)/is_builtin.c

DATA_MANAGEMENT_SRC = $(DATA_MANAGEMENT_DIR)/alloc_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_ast.c \
                      $(DATA_MANAGEMENT_DIR)/cleanup_execve.c \
                      $(DATA_MANAGEMENT_DIR)/xfree.c \
                      $(DATA_MANAGEMENT_DIR)/xmalloc.c \
                      $(DATA_MANAGEMENT_DIR)/list_utils_last.c \
                      $(DATA_MANAGEMENT_DIR)/list_utils_add.c \
                      $(DATA_MANAGEMENT_DIR)/xclose.c

ENV_MANAGEMENT_SRC = $(ENV_MANAGEMENT_DIR)/ft_getenv.c \
                     $(ENV_MANAGEMENT_DIR)/getenv_utiles.c \
                     $(ENV_MANAGEMENT_DIR)/init_env.c

EXECUTER_SRC = $(EXECUTER_DIR)/executor.c

AST_TRAVERSAL_SRC = $(AST_TRAVERSAL_DIR)/ast_traversal.c \
                    $(AST_TRAVERSAL_DIR)/do_redirection.c \
                    $(AST_TRAVERSAL_DIR)/exec_command.c \
                    $(AST_TRAVERSAL_DIR)/exec_pipe.c \
                    $(AST_TRAVERSAL_DIR)/exec_subshell.c\
                    $(AST_TRAVERSAL_DIR)/append_argv.c\
                    $(AST_TRAVERSAL_DIR)/handle_redirection_error.c\
                    $(AST_TRAVERSAL_DIR)/process_command.c\
                    $(AST_TRAVERSAL_DIR)/restore_stdio.c
TOKEN_EXPANSION_SRC = $(TOKEN_EXPANSION_DIR)/expand_tokens.c

GEN_WORD_SRC =    $(GEN_WORD_DIR)/gen_word.c \

HEREDOC_SRC = \
              $(HEREDOC_DIR)/ext_unit.c \
              $(HEREDOC_DIR)/join_value.c \
              $(HEREDOC_DIR)/ft_mkstmp.c \
              $(HEREDOC_DIR)/make_file_heredoc.c \
              $(HEREDOC_DIR)/make_pipe_heredoc.c \
              $(HEREDOC_DIR)/get_document.c \
              $(HEREDOC_DIR)/heredoc_value_expansion.c \

SEARCH_EXEC_SRC = $(SEARCH_EXEC_DIR)/search_and_exec.c \
                  $(SEARCH_EXEC_DIR)/run_external_cmd.c \
                  $(SEARCH_EXEC_DIR)/find_command_path.c \
                  $(SEARCH_EXEC_DIR)/expand_value.c \
                  $(SEARCH_EXEC_DIR)/ft_itoa.c \
                  $(SEARCH_EXEC_DIR)/exec_with_slash.c \
                  $(SEARCH_EXEC_DIR)/string_utiles.c \
                  $(SEARCH_EXEC_DIR)/env_utiles.c \
                  $(SEARCH_EXEC_DIR)/generate_env.c \
                  $(SEARCH_EXEC_DIR)/expand_dollar.c

LEXER_SRC = $(LEXER_DIR)/lexer.c

LEXER_UTILS_SRC = $(LEXER_UTILS_DIR)/prepend_tokens.c \
                  $(LEXER_UTILS_DIR)/get_meta_char.c \
                  $(LEXER_UTILS_DIR)/get_token_type.c \
                  $(LEXER_UTILS_DIR)/get_token_type_utils.c \
                  $(LEXER_UTILS_DIR)/is_doller_token.c \
                  $(LEXER_UTILS_DIR)/is_quote.c \
                  $(LEXER_UTILS_DIR)/set_quote_flag.c \
                  $(LEXER_UTILS_DIR)/set_token_type.c \
                  $(LEXER_UTILS_DIR)/word_cat.c \
                  $(LEXER_UTILS_DIR)/lexer_handlers_meta.c \
                  $(LEXER_UTILS_DIR)/lexer_handlers_word.c

EXPAND_BRANCH_DIR = $(WORD_EXPANSION_DIR)/expand_branch
EXPAND_BRANCH_SRC = \
       $(EXPAND_BRANCH_DIR)/expand_doller_word.c\
       $(EXPAND_BRANCH_DIR)/expand_plain_word.c\
       $(EXPAND_BRANCH_DIR)/get_word_string.c

WORD_EXPANSION_SRC = \
                     $(WORD_EXPANSION_DIR)/expand_dollar.c \
                     $(EXPAND_BRANCH_SRC)

WORD_LIST_SRC = $(WORD_LIST_DIR)/gen_word_list.c \
                $(WORD_LIST_DIR)/quoted_word_handler.c \
                $(WORD_LIST_DIR)/unquoted_word_handler.c\
                $(WORD_LIST_DIR)/word_node_utils.c

PARSER_SRC = $(PARSER_DIR)/fucking_gen_tree.c \
             $(PARSER_DIR)/gen_tree_utils.c \
             $(PARSER_DIR)/gen_command_node.c \
             $(PARSER_DIR)/gen_command_utils.c \
             $(PARSER_DIR)/gen_op_sep_sub_node.c \
             $(PARSER_DIR)/token_type_identification.c \
             $(PARSER_DIR)/check_parenthesis.c \
             $(PARSER_DIR)/check_parenthesis_utils.c \
             $(PARSER_DIR)/syntax_checker.c \
             $(PARSER_DIR)/syntax_error.c \
             $(PARSER_DIR)/strip_quote.c

SIGNAL_MANAGEMENT_SRC = $(SIGNAL_MANAGEMENT_DIR)/signal_management.c

# ALL SOURCE FILES
SRCS = $(MAIN_SRC) \
       $(BUILTIN_SRC) \
       $(BUILTIN_UTILS_SRC) \
       $(DATA_MANAGEMENT_SRC) \
       $(ENV_MANAGEMENT_SRC) \
       $(EXECUTER_SRC) \
       $(AST_TRAVERSAL_SRC) \
	$(TOKEN_EXPANSION_SRC) \
       $(HEREDOC_SRC) \
       $(SEARCH_EXEC_SRC) \
       $(LEXER_SRC) \
       $(LEXER_UTILS_SRC) \
       $(WORD_EXPANSION_SRC) \
       $(GEN_WORD_SRC) \
       $(WORD_LIST_SRC) \
       $(PARSER_SRC) \
       $(PARSER_UTILS_SRC) \
       $(SIGNAL_MANAGEMENT_SRC)

OBJS = $(SRCS:.c=.o)

# LIBFT
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# COMPILATION RULES
all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
$(NEW_RL_LIB):
	$(MAKE) -C $(NEW_RL_DIR)

$(NAME): $(OBJS) $(NEW_RL_LIB) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(NEW_RL_LIB) $(LIBFT) -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(NEW_RL_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(NEW_RL_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
