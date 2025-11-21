/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:12 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 22:27:57 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static int	open_and_dup(const char *filename, int flags, mode_t perm,
		int io_fd)
{
	int	fd;

	if (!filename)
		return (-1);
	if (perm == 0)
		fd = open(filename, flags);
	else
		fd = open(filename, flags, perm);
	if (fd < 0)
		return (-1);
	if (dup2(fd, io_fd) < 0)
		return (close(fd), -1);
	return (close(fd), 0);
}

static int	do_redirect_heredoc(t_redir *hd)
{
	// printf("do_redirection_heredoc() called.\n");
	if (dup2(hd->tmp_fd, STDIN_FILENO) < 0)
		return (-1);
	// printf("do_redirect_heredoc() dup2() success.\n");
	//debug
	// char buf[100] = {0};
	// read(STDIN_FILENO, buf, 100);
	// printf("%s\n", buf);
	// read(hd->tmp_fd, buf, 100);
	// printf("%s\n", buf);
	xclose(hd->tmp_fd);
	return (0);
}

static int	handle_redir_in(t_redir *cur)
{
	return (open_and_dup(cur->filename, O_RDONLY, 0, STDIN_FILENO));
}

static int	handle_redir_out(t_redir *cur)
{
	if (cur->type == REDIR_OUT)
		return (open_and_dup(cur->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644,
				STDOUT_FILENO));
	else
		return (open_and_dup(cur->filename, O_WRONLY | O_CREAT | O_APPEND, 0644,
				STDOUT_FILENO));
}

int	do_redirection(t_ast *node, t_shell *shell)
{
	t_redir	*cur;

	if (!node || !node->cmd)
		return (shell->last_exit_status = 1, -1);
	if (!node->cmd->redir)
		return (0);
	// printf("do_rediretcion() called.\n");
	// printf("redir->type: %d\n", node->cmd->redir->type);
	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN && handle_redir_in(cur) < 0)
			return (shell->last_exit_status = 1, -1);
		else if (cur->type == REDIR_HEREDOC && do_redirect_heredoc(cur) < 0)//
			return (shell->last_exit_status = 1, -1);
		else if ((cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
			&& handle_redir_out(cur) < 0)
			return (shell->last_exit_status = 1, -1);
		cur = cur->next;
	}
	return (0);
}
