#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell_types.h"

// executor entry
int				ast_traversal(t_ast *node, t_shell *shell);

// redirection and command execution helpers
int				do_redirection(t_ast *node);
int				exec_command(t_ast *node, t_shell *shell);
int				exec_pipe(t_ast *node, t_shell *shell);
int				exec_subshell(t_ast *node, t_shell *shell);

// heredoc utils
unsigned int	ft_rand(unsigned int *seed);
char			*ft_mkstmp(char *template);
int				ft_mkstmpfd(char *template, char **filename);
int				make_heredoc(t_redir *hd);

// search and exec
void			search_and_exec(t_shell *shell, char **cmd_args);
void			search_in_path_and_exec(t_shell *shell, char **cmd_args);
char			*find_command_path(t_shell *shell, char *cmd);
char			**generate_envp(t_env *env_list);

// Variable expansion (at execution time)
char			**gen_argv(t_argv *argv_list, t_shell *shell);
char			*expand_word(char *word, t_shell *shell);
char			*ft_itoa(int n);

#endif