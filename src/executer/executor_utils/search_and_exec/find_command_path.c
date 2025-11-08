/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_command_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 22:49:57 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/08 11:45:59 by ymizuniw         ###   ########.fr       */
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
	size_t	i;

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

static char	*build_pwd_path(t_shell *shell, char *cmd)
{
	char	*full_path;
	size_t	len_pwd;
	size_t	len_cmd;

	len_pwd = ft_strlen(shell->pwd);
	len_cmd = ft_strlen(cmd);
	full_path = malloc(len_pwd + len_cmd + 2);
	if (!full_path)
		return (NULL);
	memcpy(full_path, shell->pwd, len_pwd);
	full_path[len_pwd] = '/';
	memcpy(full_path + len_pwd + 1, cmd, len_cmd);
	full_path[len_pwd + len_cmd + 1] = '\0';
	return (full_path);
}

static char	*handle_no_path(t_shell *shell, char *cmd)
{
	char	*full_path;

	full_path = build_pwd_path(shell, cmd);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	shell->last_exit_status = 127;
	write(2, "minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": No such file or directory\n", 28);
	return (NULL);
}

static int	is_valid_executable(char *path)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
		return (0);
	if (stat(path, &st) != 0)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	if (access(path, X_OK) != 0)
		return (0);
	return (1);
}

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*full_path;
	size_t	i;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (full_path && is_valid_executable(full_path))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_command_path(t_shell *shell, char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || !cmd[0])
		return (NULL);
	path_env = get_env_value(shell->env_list, "PATH");
	if (!path_env || !*path_env)
		return (handle_no_path(shell, cmd));
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, cmd);
	free_split(paths);
	if (result)
		return (result);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 20);
	shell->last_exit_status = 127;
	return (NULL);
}
