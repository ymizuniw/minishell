/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:38:38 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	handle_backspace(char *buf, size_t *len)
{
	if (*len > 0)
	{
		(*len)--;
		buf[*len] = '\0';
		write(STDOUT_FILENO, "\b \b", 3);
	}
}

void	handle_printable(char *buf, size_t *len, char c)
{
	if (*len >= 1023)
		return ;
	buf[*len] = c;
	(*len)++;
	buf[*len] = '\0';
	write(STDOUT_FILENO, &c, 1);
}
