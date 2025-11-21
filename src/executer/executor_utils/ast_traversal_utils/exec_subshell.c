/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:22 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 21:20:16 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	exec_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_exit_status = 1;
		return (signal_initializer(shell->interactive), 1);
	}
	if (pid == 0)
	{
		set_sig_dfl();
		if (node->cmd && node->cmd->redir)
		{
			if (do_redirection(node, shell) < 0)
				exit(shell->last_exit_status);
		}
		if (node && node->subtree)
			ast_traversal(node->subtree, shell);
		exit(shell->last_exit_status);
	}
	handle_child(&shell->last_exit_status, pid);
	signal_initializer(shell->interactive);
	return (0);
}
