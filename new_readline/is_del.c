/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_del.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:19:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:19:09 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

bool	is_del(char one_char)
{
	return (one_char == '\b' || (unsigned char)one_char == 0x7f);
}
