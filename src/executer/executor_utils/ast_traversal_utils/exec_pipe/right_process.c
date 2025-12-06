/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   right_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 17:36:36 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/06 17:37:07 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	exec_pipe_right_child(int *pip, t_ast *node, t_shell *shell)
{
	set_sig_dfl();
	shell->in_pipe_child = true;
	close(pip[0]);
	if (dup2(pip[1], STDOUT_FILENO) < 0)
		ft_exit(NULL, 1, shell);
	close(pip[1]);
	ast_traversal(node->right, shell);
	free_shell(shell);
	exit(shell->last_exit_status);
}

static int	fork_pipe_right(int *pip, t_ast *node, t_shell *shell)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return (-1);
	}
	if (right_pid == 0)
		exec_pipe_right_child(pip, node, shell);
	return (right_pid);
}

pid_t	fork_right_process(int pip[2], t_ast *node, t_shell *shell)
{
	pid_t	right_pid;

	right_pid = fork_pipe_right(pip, node, shell);
	if (right_pid < 0)
	{
		signal_initializer(shell->interactive);
		return (-1);
	}
	return (right_pid);
}
