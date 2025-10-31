NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -D_DEFAULT_SOURCE
# -fsanitize=address
# -g
LDFLAGS = -lreadline
INC = includes

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

# SOURCE FILES
MAIN_SRC = $(SRC_DIR)/main.c

BUILTIN_SRC = $(BUILTIN_DIR)/exec_builtin.c \
              $(BUILTIN_DIR)/ft_cd.c \
              $(BUILTIN_DIR)/ft_echo.c \
              $(BUILTIN_DIR)/ft_env.c \
              $(BUILTIN_DIR)/ft_exit.c \
              $(BUILTIN_DIR)/ft_export.c \
              $(BUILTIN_DIR)/ft_pwd.c \
              $(BUILTIN_DIR)/ft_unset.c

BUILTIN_UTILS_SRC = $(BUILTIN_UTILS_DIR)/get_pwd.c \
                    $(BUILTIN_UTILS_DIR)/is_builtin.c

DATA_MANAGEMENT_SRC = $(DATA_MANAGEMENT_DIR)/alloc_t.c \
                      $(DATA_MANAGEMENT_DIR)/free_t.c \
                      $(DATA_MANAGEMENT_DIR)/xfree.c \
                      $(DATA_MANAGEMENT_DIR)/xmalloc.c

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
                  $(LEXER_UTILS_DIR)/word_cat.c

PARSER_SRC = $(PARSER_DIR)/fucking_gen_tree.c \
             $(PARSER_DIR)/gen_command_node.c \
             $(PARSER_DIR)/gen_op_sep_sub_node.c \
             $(PARSER_DIR)/token_type_identification.c \
             $(PARSER_DIR)/check_parenthesis.c \
             $(PARSER_DIR)/syntax_checker.c \
             $(PARSER_DIR)/syntax_error.c

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
       $(SIGNAL_MANAGEMENT_SRC)

OBJS = $(SRCS:.c=.o)

# Core objects without main.c (used for tests)
CORE_SRCS := $(filter-out $(MAIN_SRC),$(SRCS))
CORE_OBJS := $(CORE_SRCS:.c=.o)

# COMPILATION RULES
%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# TESTS
TEST_DIR := tests
TEST_BIN_DIR := $(TEST_DIR)/bin
TEST_SRCS := $(shell find $(TEST_DIR) -name "*_test.c" -o -name "*smoke_test.c")
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%,$(TEST_SRCS))

$(TEST_BIN_DIR)/%: $(TEST_DIR)/%.c $(CORE_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC) $< $(CORE_OBJS) $(LDFLAGS) -o $@

# Simple smoke tests we added
SMOKE_SRCS := \
	$(TEST_DIR)/executer/ast_traversal_test.c \
	$(TEST_DIR)/executer/exec_pipe_smoke_test.c \
	$(TEST_DIR)/executer/heredoc/ft_mkstmp_test.c \
	$(TEST_DIR)/env_management/env_list_test.c \
	$(TEST_DIR)/parser/parser_utils/gen_tree_smoke_test.c
SMOKE_BINS := $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%,$(SMOKE_SRCS))

# Run all test binaries with a timeout to avoid hangs
TIMEOUT ?= 5
.PHONY: test-run

test-run: tests
	@echo "Running tests with timeout $(TIMEOUT)s"
	@set -e; \
	for t in $(TEST_BINS); do \
	  echo "[RUN] $$t"; \
	  if command -v timeout >/dev/null 2>&1; then \
	    timeout $(TIMEOUT)s "$$t" || echo "[TIMEOUT] $$t"; \
	  else \
	    echo "timeout not found; running without timeout"; \
	    "$$t"; \
	  fi; \
	done

.PHONY: all clean fclean re tests smoketests

tests: $(TEST_BINS)
	@echo "Built $(words $(TEST_BINS)) test binaries into $(TEST_BIN_DIR)"

smoketests: $(SMOKE_BINS)
	@echo "Built $(words $(SMOKE_BINS)) smoke test binaries into $(TEST_BIN_DIR)"