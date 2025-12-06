/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cntrl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 03:31:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 16:01:37 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"
#include <termcap.h>

int	process_cntrl(t_new_readline *nr, char one_char)
{
	(void)nr;
	if (one_char == 0x0C)
	{
		write(1, "L", 1);
		return (1);
	}
	return (5);
}
