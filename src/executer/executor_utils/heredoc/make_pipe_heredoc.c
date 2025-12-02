/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_pipe_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:16:47 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/get_next_line.h"
#include <minishell.h>

int	make_pipe_heredoc(char *document, size_t document_len)
{
	int	pip[2];

	if (pipe(pip) < 0)
		return (-1);
	write(pip[1], document, document_len);
	xfree(document);
	xclose(pip[1]);
	return (pip[0]);
}
