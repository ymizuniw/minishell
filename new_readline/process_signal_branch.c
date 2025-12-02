/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_signal_branch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 21:00:55 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/03 03:42:59 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

//buf_len==0;line==NULL;exit();
//buf_len>=1;do nothing.
static int	process_eot(t_new_readline *nr, char **line)
{
	if (nr->buf_len == 0)
		*line = NULL;
	else
		return (5);
	return (2);
}

static int	process_etx(t_new_readline *nr, char **line, bool is_heredoc)
{
	nr->buf_len = 0;
	nr->buf[0] = '\0';
	write(STDOUT_FILENO, "\n", 1);
	if (is_heredoc)
		*line = NULL;
	else
	{
		*line = ft_strdup("");
		if (*line == NULL)
			return (-1);
	}
	return (2);
}

int	process_signal_branch(t_new_readline *nr, char **line, char one_char,
		bool is_heredoc)
{
	int	sig_res;

	sig_res = process_signal(one_char);
	if (sig_res == 0)
		return (process_eot(nr, line));
	else if (sig_res == 1)
		return (process_etx(nr, line, is_heredoc));
	return (0);
}
