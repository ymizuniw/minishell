/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/19 21:37:09 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include "../../../../includes/minishell.h"

static int	make_pipe_heredoc(char *document, size_t document_len)
{
	int		herepipe[2];
	ssize_t	wb;

	if (pipe(herepipe) < 0)
		return (perror("pipe"), xfree(document), -1);
	wb = write(herepipe[1], document, document_len);
	close(herepipe[1]);
	xfree(document);
	if (wb != (ssize_t)document_len)
		return (close(herepipe[0]), -1);
	return (herepipe[0]);
}

static int	make_file_heredoc(char *document)
{
	char	*filename;
	int		tmp_fd;
	int		fd;

	filename = NULL;
	tmp_fd = ft_mkstmpfd("tmp/", &filename);
	xfree(document);
	if (tmp_fd < 0)
		return (-1);
	close(tmp_fd);
	fd = open(filename, O_RDONLY);
	xfree(filename);
	return (fd);
}

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
	if (document_len + 1 <= HERE_PIPE_SIZE)
		return (make_pipe_heredoc(document, document_len));
	else
		return (make_file_heredoc(document));
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
	if (node && node->cmd && node->cmd->redir)
	{
		if (process_one_heredoc(shell, node->cmd->redir) < 0)
			return (-1);
	}
	return (1);
}

/*
	caller side:
		if (process-all_heredoc(shell, node)<0)
			//err path.

*/