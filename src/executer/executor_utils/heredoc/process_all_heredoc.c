/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_all_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:29:13 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/08 14:02:13 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	process_redir_list(t_shell *shell, t_ast *node)
{
	t_redir	*cur_redir;

	if (node && node->cmd && node->cmd->redir)
	{
		cur_redir = node->cmd->redir;
		while (cur_redir)
		{
			if (cur_redir->type == REDIR_HEREDOC)
				if (process_one_heredoc(shell, cur_redir) < 0)
					return (-1);
			cur_redir = cur_redir->next;
		}
	}
	return (1);
}

int	process_one_heredoc(t_shell *shell, t_redir *redir)
{
	int	tmp_fd;

	tmp_fd = make_heredoc(redir, shell);
	if (tmp_fd < 0)
		return (-1);
	redir->tmp_fd = tmp_fd;
	return (1);
}

int	process_all_heredoc(t_shell *shell, t_ast *node)
{
	if (node == NULL || shell == NULL)
		return (0);
	if (node->right)
	{

		process_all_heredoc(shell, node->right);
		// if (process_all_heredoc(shell, node->right) < 0)
		// 	return (-1);
	}
	if (node->left)
	{
		process_all_heredoc(shell, node->left);
		// if (process_all_heredoc(shell, node->left) < 0)
		// 	return (-1);
	}
	if (node->type == NODE_SUBSHELL)
	{
		process_all_heredoc(shell, node->subtree);
		// if (process_all_heredoc(shell, node->subtree) < 0)
		// 	return (-1);
	}
	if (process_redir_list(shell, node) < 0)
	{
		ft_putstr_fd("heredoc failed or EOF sent.\n", STDERR_FILENO);
	}
	return (1);
}
