/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_del.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:42 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 00:10:23 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_del(t_new_readline *nr, char one_char)
{
	size_t	pos;

	(void)one_char;
	if (nr->buf_len == 0 || nr->cursor_buf_pos == 0)
		return (1);
	pos = nr->cursor_buf_pos - 1;
	//if tab: del size differs.
	//if tab comes, adjust the coordination of x-y by sizeof(tab).
	//actually, del key shall delete the count corresponds to the size of character.
	//However, if ft_memmove() moves buf position, and redraw the full line,
	//delete all the line and redrawing the prompt+entered line is sufficient. 
	ft_memmove(&nr->buf[pos], &nr->buf[pos + 1], nr->buf_len - pos - 1);
	nr->buf_len--;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_buf_pos--;
	redraw_line(nr);
	return (1);
}
