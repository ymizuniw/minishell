/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 22:43:35 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include "../../../../includes/minishell.h"

static int	make_file_heredoc(char *document, size_t document_len)
{
	int		tmp_fd;
	ssize_t	wb;
	char filename[22];

	ft_memset(filename,0, 22);
	if (generate_random_template(filename)< 0)
		return (-1);
	tmp_fd = open(filename, O_RDWR|O_EXCL|O_CREAT, 0600);
	if (tmp_fd < 0)
		return (-1);
	if (document == NULL)
	{
		xfree(document);
		return (tmp_fd);
	}
	wb = write(tmp_fd, document, document_len);
	if (wb < 0)
		return (xfree(document), xclose(tmp_fd), -1);
	if ((size_t)wb != document_len)
		return (xfree(document), xclose(tmp_fd), -1);
	xfree(document);
	close(tmp_fd);
	tmp_fd = open(filename, O_RDONLY, 0600);
	unlink(filename);
	if (tmp_fd<0)
		return (-1);
	return (tmp_fd);
}

int make_pipe_heredoc(char *document, size_t document_len)
{
	int pip[2];

	if (pipe(pip)<0)
		return (-1);
	write(pip[1], document, document_len);
	xfree(document);
	xclose(pip[1]);
	return (pip[0]);
}

// maybe, just returning empty tmpfile descriptor is better.
int	make_heredoc(t_redir *hd, t_shell *shell)
{
	char	*document;
	size_t	document_len;
	int		fd;

	document = NULL;
	document_len = 0;
	if (get_document(hd, &document, &document_len, shell) < 0)
		return (xfree(document), -1);
	if (document_len == 0)
	{
		fd = open("/dev/null", O_RDONLY);
		xfree(document);
		return (fd);
	}
	if (document_len>HERE_PIPE_SIZE)
		return (make_file_heredoc(document, document_len));
	return (make_pipe_heredoc(document, document_len));
}

// set redir->tmp_fd;
int	process_one_heredoc(t_shell *shell, t_redir *redir)
{
	int	tmp_fd;

	tmp_fd = -1;
	tmp_fd = make_heredoc(redir, shell);
	if (tmp_fd < 0)
		return (-1);
	redir->tmp_fd = tmp_fd;
	return (1);
}

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

int	process_all_heredoc(t_shell *shell, t_ast *node)
{
	if (node == NULL || shell == NULL)
		return (0);
	if (node->right)
	{
		if (process_all_heredoc(shell, node->right) < 0)
			return (-1);
	}
	if (node->left)
	{
		if (process_all_heredoc(shell, node->left) < 0)
			return (-1);
	}
	if (process_redir_list(shell, node) < 0)
		return (-1);
	return (1);
}
