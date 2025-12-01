/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 19:30:42 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	close_fds(int fds[2])
{
	if (fds[0] >= 0)
		xclose(fds[0]);
	if (fds[1] >= 0)
		xclose(fds[1]);
}

void	dup_fail_exit(int fds[2], t_shell *shell)
{
	close_fds(fds);
	perror("dup");
	shell->last_exit_status = 1;
	ft_exit(NULL, shell->last_exit_status, shell);
}

int	exec_command(t_ast *node, t_shell *shell)
{
	int	redir_ret;
	int	fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[0] < 0 || fds[1] < 0)
		dup_fail_exit(fds, shell);
	shell->stdin_backup = fds[0];
	shell->stdout_backup = fds[1];
	if (!node || !node->cmd)
	{
		restore_stdio(fds[0], fds[1]);
		shell->stdin_backup = -1;
		shell->stdout_backup = -1;
		return (-1);
	}
	redir_ret = do_redirection(node, shell);
	if (redir_ret < 0)
		return (handle_redirection_error(node, fds[0], fds[1], shell));
	process_command(node, shell);
	restore_stdio(fds[0], fds[1]);
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	return (0);
}
