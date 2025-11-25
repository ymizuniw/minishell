/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_document.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:30 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/26 00:49:22 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include "../../../../includes/minishell.h"

static bool	read_heredoc_line(t_shell *shell, char **line, char **line_copy,
		int is_interactive)
{
	size_t	len;

	if (is_interactive)
	{
		*line = ft_readline(shell, ">", NULL);
		if (!*line)
			return (false);
		*line_copy = *line;
	}
	else
	{
		*line = get_next_line(STDIN_FILENO);
		if (!*line)
			return (false);
		len = ft_strlen(*line);
		if (len > 0 && (*line)[len - 1] == '\n')
			(*line)[len - 1] = '\0';
		*line_copy = *line;
	}
	return (true);
}

static int	process_heredoc_line(char **document, size_t *document_len,
		char *line, t_heredoc_params *params)
{
	char	*value;
	size_t	line_len;
	size_t	value_len;

	line_len = 0;
	if (line)
		line_len = ft_strlen(line);
	value = heredoc_value_expansion(line, params->delim_quoted, line_len,
			params->shell);
	if (!value)
		return (free(line), -1);
	value_len = 0;
	if (value)
		value_len = ft_strlen(value);
	if (!join_value(document, value, *document_len, value_len))
		return (free(line), free(value), -1);
	*document_len += value_len;
	if (!join_value(document, "\n", *document_len, 1))
		return (free(line), free(value), -1);
	*document_len += 1;
	return (free(line), free(value), 1);
}

static void	free_line_mode(char *line, char *line_copy, bool is_interactive)
{
	if (is_interactive)
		free(line_copy);
	else
		free(line);
}

int	get_document(t_redir *hd, char **document, size_t *document_len,
		t_shell *shell)
{
	char				*line;
	char				*line_copy;
	t_heredoc_params	params;
	int					is_interactive;

	params = (t_heredoc_params){hd->filename, hd->delim_quoted, shell};
	line = NULL;
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (!read_heredoc_line(shell, &line, &line_copy, is_interactive))
			break ;
		if (ft_strcmp(line_copy, params.delim) == 0)
		{
			free_line_mode(line, line_copy, is_interactive);
			return (1);
		}
		if (process_heredoc_line(document, document_len, line_copy,
				&params) < 0)
		{
			free_line_mode(line, line_copy, is_interactive);
			return (-1);
		}
	}
	return (-1);
}
