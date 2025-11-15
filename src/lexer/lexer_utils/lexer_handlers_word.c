/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers_word.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 15:09:47 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	prepend_tokens(t_token *token_head, t_token *new_token);
size_t	word_cat(char **word, size_t word_len, char const *input,
			size_t input_len, size_t *idx);
bool	is_doller_token(const char *str);
int		handle_meta_char(t_token *token_head, const char *input, size_t *idx);

int	handle_doller(t_token *token_head, size_t *idx)
{
	t_token	*new;

	new = alloc_token();
	if (!new)
		return (-1);
	ft_memset(new, 0, sizeof(t_token));
	new->type = TK_DOLLER;
	new->value = ft_strdup("$");
	if (!new->value)
		return (-1);
	prepend_tokens(token_head, new);
	(*idx)++;
	return (1);
}

int	handle_word(t_token *token_head, char const *input, size_t input_len,
		size_t *idx)
{
	t_token	*new;
	char	*word;

	new = alloc_token();
	if (!new)
		return (-1);
	ft_memset(new, 0, sizeof(t_token));
	word = NULL;
	if (word_cat(&word, 0, (char *)input, input_len, idx) == 0)
	{
		free(new);
		free(word);
		return (-1);
	}
	new->type = TK_WORD;
	new->value = word;
	new->next = NULL;
	prepend_tokens(token_head, new);
	return (1);
}

int	handle_word_and_doller(t_token *token_head, char const *input,
		size_t input_len, size_t *idx)
{
	if (is_doller_token(&input[*idx]))
	{
		if (handle_doller(token_head, idx) < 0)
			return (-1);
	}
	else
	{
		if (handle_word(token_head, input, input_len, idx) < 0)
			return (-1);
	}
	return (1);
}

int	handle_operators_and_words(t_token *token_head, char const *input,
		size_t input_len, size_t *idx)
{
	t_metachar	meta;

	meta = is_meta_char(input[*idx]);
	if (meta != MT_OTHER)
	{
		if (handle_meta_char(token_head, input, idx) < 0)
			return (-1);
	}
	else
	{
		if (handle_word_and_doller(token_head, input, input_len, idx) < 0)
			return (-1);
	}
	return (1);
}
