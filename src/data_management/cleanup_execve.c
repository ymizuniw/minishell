/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_execve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:16 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/30 22:23:47 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	free_env_list(t_env *env_list);
// void	free_ast_tree(t_ast *node);
// void	free_token_list(t_token *p);

void	cleanup_before_execve(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
	if (shell->hist)
		free_hist_box(shell->hist->hist_box);
	free_env_list(shell->env_list);
	free_ast_tree(shell->root);
	free_token_list(shell->token_list);
	xfree(shell->line_ptr);
	xfree(shell->pwd);
}
