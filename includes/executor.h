#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell_types.h"

// executor entry
int				ast_traversal(t_ast *node, t_shell *shell, bool execute);

// redirection and command execution helpers
int				do_redirection(t_ast *node);
int				exec_command(t_ast *node, t_shell *shell, int execute);
int				exec_pipe(t_ast *node, t_shell *shell, bool execute);
int				exec_subshell(t_ast *node, t_shell *shell, bool execute);

// heredoc utils
unsigned int	ft_rand(unsigned int *seed);
char			*ft_mkstmp(char *template);
int				ft_mkstmpfd(char *template, char **filename);
int				make_heredoc(t_redir *hd);

// search and exec (optional; may be unimplemented)
int				search_and_exec(char *cmd, t_env *env_list);

#endif