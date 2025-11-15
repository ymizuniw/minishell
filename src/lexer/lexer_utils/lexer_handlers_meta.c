/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers_meta.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 16:34:32 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void			prepend_tokens(t_token *token_head, t_token *new_token);
t_token_type	get_token_type(char *input, size_t *idx);

int	handle_newline(size_t *idx)
{
	(*idx)++;
	return (1);
}

int	handle_meta_char(t_token *token_head, const char *input, size_t *idx)
{
	t_token	*new;
	size_t	start_idx;

	new = alloc_token();
	if (!new)
		return (-1);
	ft_memset(new, 0, sizeof(t_token));
	start_idx = *idx;
	new->type = get_token_type((char *)input, idx);
	new->value = ft_strndup(&input[start_idx], *idx - start_idx);
	if (!new->value)
	{
		free(new);
		return (-1);
	}
	prepend_tokens(token_head, new);
	return (1);
}

int	handle_internal_separator(t_token *token_head, char const *input,
		size_t *idx)
{
	if (input[*idx] && input[*idx] == '\n')
	{
		token_head->count_newline++;
		if (handle_newline(idx) < 0)
			return (-1);
	}
	if (input[*idx] && ft_isspace((unsigned char)input[*idx]))
		(*idx)++;
	return (1);
}

int	handle_eof(t_token *token_head)
{
	t_token	*new;

	new = alloc_token();
	if (!new)
		return (-1);
	new->type = TK_EOF;
	new->value = ft_strdup("");
	new->next = NULL;
	token_add_back(&token_head, new);
	return (1);
}
