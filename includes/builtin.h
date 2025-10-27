#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell_types.h"

bool	is_builtin(const char *cmd);
char	*get_pwd(void);

void	ft_builtin(t_shell *shell, char **cmd);
void	ft_echo(char **cmd);
void	ft_cd(char **cmd, t_shell *shell, int fd);
void	ft_pwd(t_shell *shell, int fd);
void	ft_export(t_shell *shell, char **cmd, int fd);
void	ft_unset(t_shell *shell, char **cmd);
void	ft_env(t_shell *shell, char **cmd);
void	ft_exit(char **cmd);

#endif