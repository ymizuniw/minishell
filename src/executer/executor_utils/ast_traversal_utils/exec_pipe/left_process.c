/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   left_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 17:38:36 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/06 17:38:54 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	exec_pipe_left_child(int *pip, t_ast *node, t_shell *shell)
{
	set_sig_dfl();
	shell->in_pipe_child = true;
	close(pip[1]);
	if (dup2(pip[0], STDIN_FILENO) < 0)
		ft_exit(NULL, shell->last_exit_status, shell);
	close(pip[0]);
	ast_traversal(node->left, shell);
	free_shell(shell);
	exit(shell->last_exit_status);
}

static int	fork_pipe_left(int *pip, t_ast *node, t_shell *shell, pid_t rpid)
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		kill(rpid, SIGTERM);
		waitpid(rpid, NULL, 0);
		return (-1);
	}
	if (left_pid == 0)
		exec_pipe_left_child(pip, node, shell);
	return (left_pid);
}

pid_t	fork_left_process(int pip[2], t_ast *node, t_shell *shell,
		pid_t right_pid)
{
	pid_t	left_pid;

	left_pid = fork_pipe_left(pip, node, shell, right_pid);
	if (left_pid < 0)
	{
		signal_initializer(shell->interactive);
		return (-1);
	}
	return (left_pid);
}
