/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_file_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/08 13:48:42 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include <minishell.h>

int	create_tmp_file(char filename[22])
{
	int	tmp_fd;

	ft_memset(filename, 0, 22);
	if (generate_random_template(filename) < 0)
		return (-1);
	tmp_fd = open(filename, O_RDWR | O_EXCL | O_CREAT, 0600);
	return (tmp_fd);
}

int	write_to_heredoc(int tmp_fd, char *document, size_t document_len)
{
	ssize_t	wb;

	if (document == NULL)
		return (1);
	wb = 0;
	wb = write(tmp_fd, document, document_len);
	if (wb < 0)
	{
		xfree(document);
		xclose(tmp_fd);
		return (-1);
	}
	if ((size_t)wb != document_len)
	{
		xfree(document);
		xclose(tmp_fd);
		return (-1);
	}
	return (1);
}

int	create_read_only_file(char filename[22])
{
	int	tmp_fd;

	tmp_fd = open(filename, O_RDONLY, 0600);
	unlink(filename);
	return (tmp_fd);
}

int	make_file_heredoc(char *document, size_t document_len)
{
	int		tmp_fd;
	char	filename[22];

	tmp_fd = create_tmp_file(filename);
	if (tmp_fd < 0)
		return (-1);
	if (write_to_heredoc(tmp_fd, document, document_len) < 0)
	{
		xfree(document);
		close(tmp_fd);
	}
	tmp_fd = create_read_only_file(filename);
	if (tmp_fd < 0)
		return (-1);
	return (tmp_fd);
}

int	make_heredoc(t_redir *hd, t_shell *shell)
{
	char	*document;
	size_t	document_len;
	int		fd;

	document = NULL;
	document_len = 0;
	get_document(hd, &document, &document_len, shell);
	// if (get_document(hd, &document, &document_len, shell) < 0)
	// {
	// 	xfree(document);
	// 	return (-1);
	// }
	if (document_len == 0)
	{
		fd = open("/dev/null", O_RDONLY);
		xfree(document);
		return (fd);
	}
	if (document_len > HERE_PIPE_SIZE)
		return (make_file_heredoc(document, document_len));
	return (make_pipe_heredoc(document, document_len));
}
