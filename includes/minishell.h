#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_types.h"
# include "data_management.h"
# include "parser.h"
# include "lexer.h"
# include "signal_management.h"

// main.c
t_token				*lexer(const char *input);
t_ast				*parser(t_token *tokens);
t_result			*executor(t_ast *ast, char **env);

#endif