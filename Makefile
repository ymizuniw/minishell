NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror -D_DEFAULT_SOURCE
INC = -Iincludes

# -g
#for Mac Environment
CPPFLAGS = -I/opt/homebrew/opt/readline/include/
LDFLAGS = -L/opt/homebrew/opt/readline/lib

#for later rename.
# lex_util_
# pse_util_
# exe_util_

# DIRECTORIES
SRC_DIR = src
BUILTIN_DIR = $(SRC_DIR)/builtin
BUILTIN_UTILS_DIR = $(BUILTIN_DIR)/builtin_utils
DATA_MANAGEMENT_DIR = $(SRC_DIR)/data_management
ENV_MANAGEMENT_DIR = $(SRC_DIR)/env_management
EXECUTER_DIR = $(SRC_DIR)/executer
EXECUTER_UTILS_DIR = $(EXECUTER_DIR)/executor_utils
AST_TRAVERSAL_DIR = $(EXECUTER_UTILS_DIR)/ast_traversal_utils
HEREDOC_DIR = $(EXECUTER_UTILS_DIR)/heredoc
SEARCH_EXEC_DIR = $(EXECUTER_UTILS_DIR)/search_and_exec
LEXER_DIR = $(SRC_DIR)/lexer
LEXER_UTILS_DIR = $(LEXER_DIR)/lexer_utils
WORD_EXPANSION_DIR = $(LEXER_DIR)/word_expansion
WORD_LIST_DIR = $(LEXER_DIR)/word_list
PARSER_DIR = $(SRC_DIR)/parser
SIGNAL_MANAGEMENT_DIR = $(SRC_DIR)/signal_management
FT_READLINE_DIR = $(SRC_DIR)/ft_readline
READLINE_UTILS_DIR = $(FT_READLINE_DIR)/readline_utils
UTILS_DIR = $(SRC_DIR)/utils
STRING_UTILS_DIR = $(UTILS_DIR)/string

# SOURCE FILES
MAIN_SRC = $(SRC_DIR)/main.c \
           $(SRC_DIR)/shell_loop_utils.c

BUILTIN_SRC = $(BUILTIN_DIR)/exec_builtin.c \
              $(BUILTIN_DIR)/ft_cd.c \
              $(BUILTIN_DIR)/ft_echo.c \
              $(BUILTIN_DIR)/ft_env.c \
              $(BUILTIN_DIR)/ft_exit.c \
              $(BUILTIN_DIR)/ft_export/ft_export.c \
              $(BUILTIN_DIR)/ft_export/export_print.c \
              $(BUILTIN_DIR)/ft_pwd.c \
              $(BUILTIN_DIR)/ft_unset.c \
              $(BUILTIN_DIR)/expand_wildcard.c

BUILTIN_UTILS_SRC = $(BUILTIN_UTILS_DIR)/get_pwd.c \
                    $(BUILTIN_UTILS_DIR)/is_builtin.c \
                    $(BUILTIN_UTILS_DIR)/wildcard_utils.c

DATA_MANAGEMENT_SRC = $(DATA_MANAGEMENT_DIR)/alloc_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_ast.c \
                      $(DATA_MANAGEMENT_DIR)/xfree.c \
                      $(DATA_MANAGEMENT_DIR)/xmalloc.c \
                      $(DATA_MANAGEMENT_DIR)/list_utils_last.c \
                      $(DATA_MANAGEMENT_DIR)/list_utils_add.c

ENV_MANAGEMENT_SRC = $(ENV_MANAGEMENT_DIR)/ft_getenv.c \
                     $(ENV_MANAGEMENT_DIR)/getenv_utiles.c \
                     $(ENV_MANAGEMENT_DIR)/init_env.c

EXECUTER_SRC = $(EXECUTER_DIR)/executor.c

AST_TRAVERSAL_SRC = $(AST_TRAVERSAL_DIR)/ast_traversal.c \
                    $(AST_TRAVERSAL_DIR)/do_redirection.c \
                    $(AST_TRAVERSAL_DIR)/exec_command.c \
                    $(AST_TRAVERSAL_DIR)/exec_pipe.c \
                    $(AST_TRAVERSAL_DIR)/exec_subshell.c

HEREDOC_SRC = $(HEREDOC_DIR)/ft_mkstmp.c \
              $(HEREDOC_DIR)/make_heredoc.c \
              $(HEREDOC_DIR)/get_document.c \
              $(HEREDOC_DIR)/heredoc_value_expansion.c

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

STRING_UTILS_SRC = $(STRING_UTILS_DIR)/string_basic.c \
                   $(STRING_UTILS_DIR)/string_compare.c \
                   $(STRING_UTILS_DIR)/string_copy.c \
                   $(STRING_UTILS_DIR)/string_split.c

LEXER_SRC = $(LEXER_DIR)/lexer.c

LEXER_UTILS_SRC = $(LEXER_UTILS_DIR)/prepend_tokens.c \
                  $(LEXER_UTILS_DIR)/get_meta_char.c \
                  $(LEXER_UTILS_DIR)/get_token_type.c \
                  $(LEXER_UTILS_DIR)/is_doller_token.c \
                  $(LEXER_UTILS_DIR)/is_quote.c \
                  $(LEXER_UTILS_DIR)/set_quote_flag.c \
                  $(LEXER_UTILS_DIR)/set_token_type.c \
                  $(LEXER_UTILS_DIR)/word_cat.c \
                  $(LEXER_UTILS_DIR)/gen_word.c

WORD_EXPANSION_SRC = $(WORD_EXPANSION_DIR)/ft_expand_word.c \
                     $(WORD_EXPANSION_DIR)/expand_dollar.c \
                     $(WORD_EXPANSION_DIR)/expand_wildcard_word.c \
                     $(WORD_EXPANSION_DIR)/expand_plain_word.c \
                     $(WORD_EXPANSION_DIR)/word_list_utils.c

WORD_LIST_SRC = $(WORD_LIST_DIR)/gen_word_list.c \
                $(WORD_LIST_DIR)/word_node_utils.c \
                $(WORD_LIST_DIR)/quoted_word_handler.c \
                $(WORD_LIST_DIR)/unquoted_word_handler.c

PARSER_SRC = $(PARSER_DIR)/fucking_gen_tree.c \
             $(PARSER_DIR)/gen_command_node.c \
             $(PARSER_DIR)/gen_op_sep_sub_node.c \
             $(PARSER_DIR)/token_type_identification.c \
             $(PARSER_DIR)/check_parenthesis.c \
             $(PARSER_DIR)/syntax_checker.c \
             $(PARSER_DIR)/syntax_error.c

FT_READLINE_SRC = $(FT_READLINE_DIR)/ft_readline.c

READLINE_UTILS_SRC = $(READLINE_UTILS_DIR)/terminal_utils.c \
                     $(READLINE_UTILS_DIR)/input_handlers.c \
                     $(READLINE_UTILS_DIR)/history_navigation.c \
                     $(READLINE_UTILS_DIR)/history_management.c

SIGNAL_MANAGEMENT_SRC = $(SIGNAL_MANAGEMENT_DIR)/signal_management.c

# ALL SOURCE FILES
SRCS = $(MAIN_SRC) \
       $(BUILTIN_SRC) \
       $(BUILTIN_UTILS_SRC) \
       $(DATA_MANAGEMENT_SRC) \
       $(ENV_MANAGEMENT_SRC) \
       $(EXECUTER_SRC) \
       $(AST_TRAVERSAL_SRC) \
       $(HEREDOC_SRC) \
       $(SEARCH_EXEC_SRC) \
       $(STRING_UTILS_SRC) \
       $(LEXER_SRC) \
       $(LEXER_UTILS_SRC) \
       $(WORD_EXPANSION_SRC) \
       $(WORD_LIST_SRC) \
       $(PARSER_SRC) \
       $(PARSER_UTILS_SRC) \
       $(SIGNAL_MANAGEMENT_SRC) \
       $(FT_READLINE_SRC) \
       $(READLINE_UTILS_SRC)

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

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) $(CPPFLAGS) -lreadline -o $(NAME)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Integration tests (non-interactive harness)
.PHONY: all clean fclean re integration-tests
integration-tests: $(NAME)
	@echo "Running integration tests (Bash harness)"
	@bash tests/integration/integration_tests.sh || exit 1
