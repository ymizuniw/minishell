/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 02:05:22 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/31 16:41:13 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell_types.h"

/* Types are defined in minishell_types.h; avoid redefining here.
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;
*/

/*
typedef struct s_shell
{
	t_env			*env_list;
	char			*pwd;
	int				fd;
	int				last_exit_status;
}					t_shell;
*/

char				*get_pwd(void);
int					count_token(char **tokens);
int					is_valid_varname(const char *var);
char				*extract_key(const char *str);
char				*extract_value(const char *str);

void				set_variable(t_shell *shell, char *key, char *value,
						int exported);
t_env				*find_env(t_env *env_list, const char *key);

void				exec_builtin(t_shell *shell, char **cmd);
void				run_builtin(t_shell *shell, char **cmd);
int					ft_echo(char **cmd, int fd);
int					ft_cd(char **cmd, t_shell *shell, int fd);
int					ft_pwd(t_shell *shell, int fd);
int					ft_export(t_shell *shell, char **cmd, int fd);
int					ft_unset(t_shell *shell, char **cmd);
int					ft_env(t_shell *shell, char **cmd);
void				ft_exit(char **cmd, int last_exit_status);

size_t				ft_strlen(const char *s);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
int					ft_strcmp(const char *s1, const char *s2);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char				**ft_split(char const *s, char c);

char				*find_command_path(t_shell *shell, char *cmd);
// void				run_external_cmd(t_shell *shell, char **cmd_args);
char				*get_env_value(t_env *env_list, const char *key);
void				search_in_path_and_exec(t_shell *shell, char **cmd_args);
void				search_and_exec(t_shell *shell, char **cmd_args);
char				**generate_envp(t_env *env_list);
bool				is_builtin(const char *cmd);

#endif
