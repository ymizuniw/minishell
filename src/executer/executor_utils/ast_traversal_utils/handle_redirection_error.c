/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirection_error.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:38:34 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:41:34 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	handle_redirection_error(t_ast *node, int stdin_k, int stdout_k,
		t_shell *shell)
{
	if (node->cmd->redir && node->cmd->redir->filename)
		perror(node->cmd->redir->filename);
	shell->last_exit_status = 1;
	restore_stdio(stdin_k, stdout_k);
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	return (1);
}
