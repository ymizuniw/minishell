/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_esc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:52 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 05:06:54 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

//return 2 if not registered key pressed.
int	process_esc(t_new_readline *nr, char one_char)
{
	char	esq_buf[3];

	(void)one_char;
	ft_memset(esq_buf, 0, sizeof(esq_buf));
	if (!read_char(&esq_buf[0]))
		return (0);
	if (!read_char(&esq_buf[1]))
		return (0);
	if (ft_strcmp(esq_buf, CSI_UP) == 0)
	{
		process_up(nr);
		return (1);
	}
	else if (ft_strcmp(esq_buf, CSI_DOWN) == 0)
	{
		process_down(nr);
		return (1);
	}
	else if (ft_strcmp(esq_buf, CSI_RIGHT) == 0)
	{
		process_right(nr);
		return (1);
	}
	else if (ft_strcmp(esq_buf, CSI_LEFT) == 0)
	{
		process_left(nr);
		return (1);
	}
	unsigned char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (c >= 'A' && c <= 'Z')
			break;
        if (c == '~')
			break;
    }
	return (1);
}
