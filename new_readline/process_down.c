/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_down.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:48 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 04:53:45 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_down(t_new_readline *nr)
{
	if (!nr->hist || nr->hist->size == 0 || nr->hist->cur == -1)
		return (0);
	nr->hist->cur = (nr->hist->cur + 1) % HIST_MAX;
	if (nr->hist->cur == nr->hist->idx)
	{
		nr->buf[0] = '\0';
		nr->buf_len = 0;
		nr->cursor_buf_pos = 0;
		nr->hist->cur = -1;
	}
	else
		copy_history_to_buffer(nr);
	redraw_line(nr);
	return (1);
}
