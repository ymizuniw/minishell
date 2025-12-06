/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 20:24:34 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 20:28:32 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*init_lexer_state(size_t *idx, size_t *input_len,
		const char *input)
{
	t_token	*head;

	if (init_token(&head) < 0)
		return (NULL);
	head->type = TK_HEAD;
	*idx = 0;
	*input_len = get_input_len(input);
	return (head);
}

int	run_lexer_loop(t_token *head, const char *input, size_t input_len)
{
	size_t	idx;

	idx = 0;
	while (idx < input_len)
	{
		if (process_whitespace(head, input, &idx) < 0)
			return (-1);
		if (idx >= input_len)
			break ;
		if (process_operator_or_word(head, input, input_len, &idx) < 0)
			return (-1);
	}
	return (0);
}

int	finish_lexer(t_token *head)
{
	return (finalize_lexer(head));
}
