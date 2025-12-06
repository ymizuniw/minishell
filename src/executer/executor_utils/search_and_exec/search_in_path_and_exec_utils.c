/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_in_path_and_exec_utils.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:58:29 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:59:06 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	validate_args(char **cmd_args)
{
	return (cmd_args && cmd_args[0]);
}

char	*resolve_path(t_shell *shell, char **cmd_args)
{
	char	*path;

	path = find_command_path(shell, cmd_args[0]);
	if (!path || !path[0])
		return (NULL);
	return (path);
}

char	**prepare_envp(t_shell *shell, char *cmd_path)
{
	char	**envp;

	envp = generate_envp(shell->env_list);
	if (!envp)
		xfree(cmd_path);
	return (envp);
}

void	run_in_pipe_child(char *cmd, char **args, char **envp, t_shell *sh)
{
	exec_in_pipe_child(cmd, args, envp, sh);
}

int	spawn_child(t_shell *shell, char *cmd_path, char **cmd_args,
		char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		xfree(cmd_path);
		free_envp(envp);
		shell->last_exit_status = 1;
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
		exec_in_fork_child(cmd_path, cmd_args, envp, shell);
	xfree(cmd_path);
	free_envp(envp);
	handle_child(&shell->last_exit_status, pid);
	return (0);
}
