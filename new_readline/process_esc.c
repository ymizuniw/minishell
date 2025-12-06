/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_esc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:52 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 16:06:58 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

static int	read_escape_sequence(char buf[3])
{
	ft_memset(buf, 0, 3);
	if (!read_char(&buf[0]))
		return (0);
	if (!read_char(&buf[1]))
		return (0);
	return (1);
}

static int	process_arrow_keys(t_new_readline *nr, const char buf[3])
{
	if (ft_strcmp(buf, CSI_UP) == 0)
	{
		process_up(nr);
		return (1);
	}
	else if (ft_strcmp(buf, CSI_DOWN) == 0)
	{
		process_down(nr);
		return (1);
	}
	else if (ft_strcmp(buf, CSI_RIGHT) == 0)
	{
		process_right(nr);
		return (1);
	}
	else if (ft_strcmp(buf, CSI_LEFT) == 0)
	{
		process_left(nr);
		return (1);
	}
	return (0);
}

int	process_esc(t_new_readline *nr, char one_char)
{
	char			esq_buf[3];
	unsigned char	c;

	(void)one_char;
	if (!read_escape_sequence(esq_buf))
		return (0);
	if (process_arrow_keys(nr, esq_buf))
		return (1);
	while (read(STDIN_FILENO, &c, 1) == 1)
	{
		if (c >= 'A' && c <= 'Z')
			break ;
		if (c == '~')
			break ;
	}
	return (1);
}
