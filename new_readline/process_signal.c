/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:13 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 04:47:29 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_signal(char one_char)
{
	if (one_char == ASC_ETX)
		return (1);
	else if (one_char == ASC_EOT)
		return (0);
	return (-1);
}
