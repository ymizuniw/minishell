/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_char.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:29 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 23:39:38 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

bool	read_char(char *one_char)
{
	ssize_t	rb;

	rb = read(STDIN_FILENO, one_char, 1);
	if (rb < 0)
	{
		perror("read");
		return (false);
	}
	return (true);
}
