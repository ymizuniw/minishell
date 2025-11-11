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
PARSER_DIR = $(SRC_DIR)/parser
SIGNAL_MANAGEMENT_DIR = $(SRC_DIR)/signal_management
FT_READLINE_DIR = $(SRC_DIR)/ft_readline

# SOURCE FILES
MAIN_SRC = $(SRC_DIR)/main.c

BUILTIN_SRC = $(BUILTIN_DIR)/exec_builtin.c \
              $(BUILTIN_DIR)/ft_cd.c \
              $(BUILTIN_DIR)/ft_echo.c \
              $(BUILTIN_DIR)/ft_env.c \
              $(BUILTIN_DIR)/ft_exit.c \
              $(BUILTIN_DIR)/ft_export.c \
              $(BUILTIN_DIR)/ft_pwd.c \
              $(BUILTIN_DIR)/ft_unset.c \
              $(BUILTIN_DIR)/expand_wildcard.c

BUILTIN_UTILS_SRC = $(BUILTIN_UTILS_DIR)/get_pwd.c \
                    $(BUILTIN_UTILS_DIR)/is_builtin.c \
                    $(BUILTIN_UTILS_DIR)/wildcard_utils.c

DATA_MANAGEMENT_SRC = $(DATA_MANAGEMENT_DIR)/alloc_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_t.c \
                      $(DATA_MANAGEMENT_DIR)/xfree.c \
                      $(DATA_MANAGEMENT_DIR)/xmalloc.c \
                      $(DATA_MANAGEMENT_DIR)/list_utils.c

ENV_MANAGEMENT_SRC = $(ENV_MANAGEMENT_DIR)/ft_getenv.c \
                     $(ENV_MANAGEMENT_DIR)/init_env.c

EXECUTER_SRC = $(EXECUTER_DIR)/executor.c

AST_TRAVERSAL_SRC = $(AST_TRAVERSAL_DIR)/ast_traversal.c \
                    $(AST_TRAVERSAL_DIR)/do_redirection.c \
                    $(AST_TRAVERSAL_DIR)/exec_command.c \
                    $(AST_TRAVERSAL_DIR)/exec_pipe.c \
                    $(AST_TRAVERSAL_DIR)/exec_subshell.c

HEREDOC_SRC = $(HEREDOC_DIR)/ft_mkstmp.c \
              $(HEREDOC_DIR)/make_heredoc.c

SEARCH_EXEC_SRC = $(SEARCH_EXEC_DIR)/search_and_exec.c \
                  $(SEARCH_EXEC_DIR)/run_external_cmd.c \
                  $(SEARCH_EXEC_DIR)/find_command_path.c \
                  $(SEARCH_EXEC_DIR)/ft_string_utils.c \
                  $(SEARCH_EXEC_DIR)/fucking_expand_value.c \
                  $(SEARCH_EXEC_DIR)/ft_itoa.c

LEXER_SRC = $(LEXER_DIR)/lexer.c

LEXER_UTILS_SRC = $(LEXER_UTILS_DIR)/prepend_tokens.c \
                  $(LEXER_UTILS_DIR)/get_meta_char.c \
                  $(LEXER_UTILS_DIR)/get_token_type.c \
                  $(LEXER_UTILS_DIR)/is_doller_token.c \
                  $(LEXER_UTILS_DIR)/is_quote.c \
                  $(LEXER_UTILS_DIR)/set_quote_flag.c \
                  $(LEXER_UTILS_DIR)/set_token_type.c \
                  $(LEXER_UTILS_DIR)/word_cat.c \
                  $(LEXER_UTILS_DIR)/gen_word/gen_word.c \
                  $(LEXER_UTILS_DIR)/gen_word/expand_wildcard.c \
                  $(LEXER_UTILS_DIR)/gen_word/expand_dollar.c

PARSER_SRC = $(PARSER_DIR)/fucking_gen_tree.c \
             $(PARSER_DIR)/gen_command_node.c \
             $(PARSER_DIR)/gen_op_sep_sub_node.c \
             $(PARSER_DIR)/token_type_identification.c \
             $(PARSER_DIR)/check_parenthesis.c \
             $(PARSER_DIR)/syntax_checker.c \
             $(PARSER_DIR)/syntax_error.c

FT_READLINE_SRC = $(FT_READLINE_DIR)/ft_readline.c

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
       $(LEXER_SRC) \
       $(LEXER_UTILS_SRC) \
       $(PARSER_SRC) \
       $(PARSER_UTILS_SRC) \
       $(SIGNAL_MANAGEMENT_SRC) \
       $(FT_READLINE_SRC)

OBJS = $(SRCS:.c=.o)

# COMPILATION RULES
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lreadline -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# Integration tests (non-interactive harness)
.PHONY: integration-tests
integration-tests: $(NAME)
	@echo "Running integration tests (Bash harness)"
	@bash tests/integration/integration_tests.sh || exit 1
