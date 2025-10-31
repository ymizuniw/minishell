/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:49:57 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/31 14:08:36 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static char	*join_path(char *dir, char *cmd)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*var;

	var = find_env(env_list, key);
	if (var)
		return (var->value);
	return (NULL);
}

static void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*find_command_path(t_shell *shell, char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
	size_t	len_cmd;
	size_t	len_pwd;

	path_env = get_env_value(shell->env_list, "PATH");
	if (!path_env || !*path_env)
	{
		len_pwd = ft_strlen(shell->pwd);
		len_cmd = ft_strlen(cmd);
		full_path = malloc(len_pwd + len_cmd + 2);
		if (!full_path)
			return (NULL);
		memcpy(full_path, shell->pwd, len_pwd);
		full_path[len_pwd] = '/';
		memcpy(full_path + len_pwd + 1, cmd, len_cmd);
		full_path[len_pwd + len_cmd + 1] = '\0';
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		shell->last_exit_status = 127;
		write(2, "bash: ", 6);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n", 28);
		return (NULL);
	}
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	write(2, "bash: ", 6);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	shell->last_exit_status = 127;
	return (NULL);
}
