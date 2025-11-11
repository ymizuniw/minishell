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
# include "wildcard.h"
# include <assert.h>
// static definition needed.

// main.c
char		*ft_readline(const char *prompt, t_hist *hist);
void		add_history(char *line, t_hist *hist);
void		free_hist_box(char *hist_box[HIST_MAX]);

t_token		*lexer(const char *input);
t_result	executor(t_ast *ast, t_shell *shell);

#endif