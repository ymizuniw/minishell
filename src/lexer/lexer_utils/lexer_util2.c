/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_util2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 20:25:04 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 20:31:12 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	init_token(t_token **token_head)
{
	*token_head = alloc_token();
	if (*token_head == NULL)
	{
		perror("token_alloc");
		return (0);
	}
	ft_memset(*token_head, 0, sizeof(t_token));
	return (1);
}

size_t	get_input_len(char const *input)
{
	if (input == NULL)
		return (0);
	else
		return (ft_strlen(input));
}

int	process_whitespace(t_token *head, const char *input, size_t *idx)
{
	if (input[*idx] && ft_isspace((unsigned char)input[*idx]))
		return (handle_internal_separator(head, input, idx));
	return (0);
}

int	process_operator_or_word(t_token *head, const char *input, size_t len,
		size_t *idx)
{
	return (handle_operators_and_words(head, input, len, idx));
}

int	finalize_lexer(t_token *head)
{
	return (handle_eof(head));
}
