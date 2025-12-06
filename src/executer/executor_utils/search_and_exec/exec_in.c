/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_in.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:59:31 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 19:00:36 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	exec_in_pipe_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	cleanup_before_execve(shell);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	exit(127);
}

void	exec_in_fork_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	set_sig_dfl();
	cleanup_before_execve(shell);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	exit(127);
}
