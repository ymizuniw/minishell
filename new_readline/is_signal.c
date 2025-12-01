/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:19 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:19:20 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

bool	is_signal(char one_char)
{
	return (one_char == ASC_ETX || one_char == ASC_EOT);
}
