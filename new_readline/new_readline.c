/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_readline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:01:04 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 21:03:38 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"
#include <signal.h>
#include <term.h>

void	init_new_readline(t_new_readline *nr, bool is_heredoc, t_hist *hist)
{
	ft_memset(nr, 0, sizeof(t_new_readline));
	ft_memset(nr->buf, 0, sizeof(nr->buf));
	if (is_heredoc)
	{
		nr->hist = NULL;
		nr->prompt = ">";
	}
	else
	{
		nr->hist = hist;
		nr->prompt = "minishell$ ";
	}
	nr->prompt_len = ft_strlen(nr->prompt);
	nr->cursor_x = nr->prompt_len;
	nr->cursor_y = 0;
	resize_window(nr);
	enable_raw_mode(&nr->original);
}

int	process_one_char(t_new_readline *nr, char **line, char one_char,
		bool is_heredoc)
{
	if (one_char == '\n')
		return (process_break_line(nr, line));
	else if (is_signal(one_char))
		return (process_signal_branch(nr, line, one_char, is_heredoc));
	else if (is_del(one_char))
		return (process_del(nr, one_char));
	else if (is_esc(one_char))
		return (process_esc(nr, one_char));
	else if (ft_isprint((unsigned char)one_char)
		|| ft_isspace((unsigned char)one_char))
		return (process_printable(nr, one_char));
	return (0);
}

char	*read_line_loop(t_new_readline *nr, bool is_heredoc)
{
	char	one_char;
	char	*line;
	bool	read_char_res;
	int		process_char_ret;

	one_char = 0;
	line = NULL;
	read_char_res = false;
	while (1)
	{
		read_char_res = read_char(&one_char);
		if (!read_char_res)
			break ;
		process_char_ret = process_one_char(nr, &line, one_char, is_heredoc);
		if (process_char_ret == -1)
		{
			xfree(line);
			line = NULL;
			break ;
		}
		if (process_char_ret == 2)
			break ;
	}
	return (line);
}

char	*new_readline(t_hist *hist, bool is_heredoc)
{
	t_new_readline	nr;
	char			*line;

	line = NULL;
	init_new_readline(&nr, is_heredoc, hist);
	write(STDOUT_FILENO, nr.prompt, nr.prompt_len);
	line = read_line_loop(&nr, is_heredoc);
	disable_raw_mode(&nr.original);
	return (line);
}
