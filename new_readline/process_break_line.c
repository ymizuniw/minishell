/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_break_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:56:45 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/01 20:57:26 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_break_line(t_new_readline *nr, char **line)
{
	write(STDOUT_FILENO, "\n", 1);
	*line = ft_strdup(nr->buf);
	if (*line == NULL)
		return (-1);
	return (2);
}
