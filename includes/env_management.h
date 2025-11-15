/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:11 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 18:35:13 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGEMENT_H
# define ENV_MANAGEMENT_H

# include "minishell_types.h"

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);

// initialize and free env list
void	init_env_from_envp(t_shell *shell, char **envp);
void	free_env_list(t_env *env_list);

char	*get_pwd(void);
int		count_token(char **tokens);
int		is_valid_varname(const char *var);
char	*extract_key(const char *str);
char	*extract_value(const char *str);

void	set_variable(t_shell *shell, char *key, char *value, int exported);
t_env	*find_env(t_env *env_list, const char *key);

#endif
