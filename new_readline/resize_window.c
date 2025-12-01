/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_window.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:48 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:20:49 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	resize_window(t_new_readline *nr)
{
	struct winsize	w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0)
	{
		nr->terminal_width = w.ws_col;
		nr->terminal_height = w.ws_row;
	}
	else
	{
		perror("ioctl");
		return (-1);
	}
	return (1);
}
