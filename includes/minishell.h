#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtin.h"
# include "data_management.h"
# include "env_management.h"
# include "executor.h"
# include "lexer.h"
# include "minishell_types.h"
# include "parser.h"
# include "signal_management.h"

// main.c
t_token		*lexer(const char *input);
t_ast		*parser(t_token **tokens);
t_result	*executor(t_ast *ast, t_shell *shell);

#endif