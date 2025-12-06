/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 20:30:30 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lexer(const char *input)
{
	t_token	*head;
	size_t	idx;
	size_t	input_len;

	head = init_lexer_state(&idx, &input_len, input);
	if (!head)
		return (NULL);
	if (run_lexer_loop(head, input, input_len) < 0)
	{
		free(head);
		return (NULL);
	}
	if (finish_lexer(head) < 0)
	{
		free(head);
		return (NULL);
	}
	return (head);
}
