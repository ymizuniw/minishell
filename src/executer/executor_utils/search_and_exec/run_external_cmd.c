/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:00:12 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/31 19:37:09 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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

	len = strlen(key) + strlen(value) + 2; // '=' and '\0'
	str = malloc(len);
	if (!str)
		return (NULL);
	snprintf(str, len, "%s=%s", key, value);
	return (str);
}

char	**generate_envp(t_env *env_list)
{
	t_env	*current;
	char	**envp;
	int		count;
	int		i;

	current = env_list;
	// count exported variables locally
	count = 0;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	current = env_list;
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
		shell->last_exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		set_sig_dfl();
		execve(cmd_path, cmd_args, envp);
		perror("execve");
		free(cmd_path);
		free_envp(envp);
		exit(127);
	}
	free(cmd_path);
	free_envp(envp);
	handle_child(&shell->last_exit_status, pid);
}
