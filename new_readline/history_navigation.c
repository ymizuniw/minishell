/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_navigation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:31 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/30 22:18:45 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

void	copy_history_to_buffer(t_new_readline *nr)
{
	size_t	hist_len;

	hist_len = 0;
	if (!nr->hist || nr->hist->cur<0)
		return ;
	if (!nr->hist->hist_box[nr->hist->cur])
		return ;
	hist_len = strlen(nr->hist->hist_box[nr->hist->cur]);
	if (hist_len > sizeof(nr->buf)-1)
		hist_len = sizeof(nr->buf) -1;
	strlcpy(nr->buf, nr->hist->hist_box[nr->hist->cur], sizeof(nr->buf));
	nr->buf_len = hist_len;
	nr->cursor_buf_pos = hist_len;
}
