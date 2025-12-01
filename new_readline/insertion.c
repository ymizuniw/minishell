/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insertion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:03 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:19:03 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	insertion(t_new_readline *nr, char one_char)
{
	size_t	second_len;

	if (nr->buf_len >= sizeof(nr->buf) - 1)
		return (0);
	second_len = nr->buf_len - nr->cursor_buf_pos;
	ft_memmove(&nr->buf[nr->cursor_buf_pos + 1], &nr->buf[nr->cursor_buf_pos],
		second_len);
	nr->buf[nr->cursor_buf_pos] = one_char;
	nr->buf_len++;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_buf_pos++;
	nr->cursor_x++;
	return (1);
}
