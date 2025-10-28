/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:00:12 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/28 23:03:39 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../builtin.h"

static void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static char	*create_env_string(const char *key, const char *value)
{
	char	*str;
	size_t	len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strlcpy(str, key, len);
	ft_strlcat(str, "=", len);
	ft_strlcat(str, value, len);
	return (str);
}

char	**generate_envp(t_env *env_list)
{
	t_env	*current;
	char	**envp;
	int		count;
	int		i;

	current = env_list;
	count = count_exported_vars(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (current)
	{
		if (current->exported)
		{
			envp[i] = create_env_string(current->key, current->value);
			if (!envp[i])
			{
				free_envp(envp);
				return (NULL);
			}
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	search_in_path_and_exec(t_shell *shell, char **cmd_args)
{
	pid_t	pid;
	char	*cmd_path;
	int		status;
	char	**envp;

	if (!cmd_args || !cmd_args[0])
		return ;
	cmd_path = find_command_path(shell, cmd_args[0]);
	if (!cmd_path)
		return ;
	envp = generate_envp(shell->env_list);
	if (!envp)
	{
		free(cmd_path);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_envp(envp);
		return ;
	}
	if (pid == 0)
	{
		execve(cmd_path, cmd_args, envp);
		perror("execve");
		free(cmd_path);
		free_envp(envp);
		shell->last_exit_status = 127;
		exit(127);
	}
	free(cmd_path);
	free_envp(envp);
	waitpid(pid, &status, 0);
}
