/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_to_buffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:57 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 23:45:21 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	write_to_buffer(t_new_readline *nr, char one_char)
{
	if (nr->buf_len >= sizeof(nr->buf) - 1)
		return (0);
	nr->buf[nr->buf_len] = one_char;
	nr->buf_len++;
	nr->cursor_buf_pos = nr->buf_len;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_x++;
	return (1);
}
