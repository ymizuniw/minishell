/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_right.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:09 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:20:11 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_right(t_new_readline *nr)
{
	if (nr->cursor_buf_pos < nr->buf_len)
	{
		nr->cursor_buf_pos++;
		nr->cursor_x++;
		write(STDOUT_FILENO, ANSI_RIGHT, sizeof(ANSI_RIGHT) - 1);
	}
	return (1);
}
