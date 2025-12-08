/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_readline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:01:04 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/08 13:23:45 by ymizuniw         ###   ########.fr       */
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
	if (one_char == 0x0c)
		return (5);
	else if (one_char == '\t')
	{
		write(STDOUT_FILENO, "\a", 1);
		return (5);
	}
	else if (ft_isprint((unsigned char)one_char)
		|| ft_isspace((unsigned char)one_char))
		return (process_printable(nr, one_char));
	else if (is_del(one_char))
		return (process_del(nr, one_char));
	else if (is_signal(one_char))
		return (process_signal_branch(nr, line, one_char, is_heredoc));
	else if (is_esc(one_char))
		return (process_esc(nr, one_char));
	else if (is_cntrl(one_char))
		return (process_cntrl(nr, one_char));
	return (0);
}

static int	read_and_process_one(t_new_readline *nr, char **line,
		bool is_heredoc)
{
	char	one_char;

	if (!read_char(&one_char))
		return (0);
	return (process_one_char(nr, line, one_char, is_heredoc));
}

char	*read_line_loop(t_new_readline *nr, bool is_heredoc)
{
	char	*line;
	int		ret;

	line = NULL;
	while (1)
	{
		ret = read_and_process_one(nr, &line, is_heredoc);
		if (ret == 0)
			break ;
		if (ret == 5)
			continue ;
		if (ret == -1)
		{
			xfree(line);
			line = NULL;
			break ;
		}
		if (ret == 2)
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
