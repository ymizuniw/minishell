/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:19 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 20:04:48 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	exec_pipe(t_ast *node, t_shell *shell)
{
	int		pip[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node || !shell || pipe(pip) < 0)
	{
		perror("pipe");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	// signal(SIGPIPE, SIG_IGN);
	right_pid = fork_right_process(pip, node, shell);
	if (right_pid < 0)
		return (-1);
	left_pid = fork_left_process(pip, node, shell, right_pid);
	if (left_pid < 0)
		return (-1);
	close(pip[0]);
	close(pip[1]);
	handle_child(&shell->last_exit_status, right_pid);
	handle_child(&shell->last_exit_status, left_pid);
	signal_initializer(shell->interactive);
	return (0);
}
