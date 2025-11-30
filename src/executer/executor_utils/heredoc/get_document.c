/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_document.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:30 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/30 21:22:20 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include "../../../../includes/minishell.h"

static bool	read_heredoc_line(t_shell *shell, char **line,
		int is_interactive)
{
	size_t	len;

	if (is_interactive)
		*line = new_readline(shell->hist, true, ">");
	else
		*line = get_next_line(STDIN_FILENO);
	if (!*line)
		return (false);
	if (!is_interactive)
	{
		len = ft_strlen(*line);
		if (len > 0 && (*line)[len - 1] == '\n')
			(*line)[len - 1] = '\0';
	}
	return (true);
}

static int	process_heredoc_line(char **document, size_t *document_len,
		char *line, t_heredoc_params *params)
{
	char	*value;
	size_t	line_len=ft_strlen(line);
	size_t	value_len;

	value = heredoc_value_expansion(line, params->delim_quoted, line_len,
			params->shell);
	if (!value)
		return (-1);
	value_len = 0;
	if (value)
		value_len = ft_strlen(value);
	if (!join_value(document, value, *document_len, value_len))
		return (free(value), -1);
	*document_len += value_len;
	if (!join_value(document, "\n", *document_len, 1))
		return (free(value), -1);
	*document_len += 1;
	return (free(value), 1);
}

int	get_document(t_redir *hd, char **document, size_t *document_len,
		t_shell *shell)
{
	char				*line;
	t_heredoc_params	params;
	int					is_interactive;

	params = (t_heredoc_params){hd->filename, hd->delim_quoted, shell};
	line = NULL;
	is_interactive = isatty(STDIN_FILENO);
	while (read_heredoc_line(shell, &line, is_interactive))
	{
		if (ft_strcmp(line, params.delim) == 0)
		{
			free(line);
			return (1);
		}
		if (process_heredoc_line(document, document_len, line,
				&params) < 0)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (-1);
}
