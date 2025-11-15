/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted_word_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:41:16 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Forward declarations for internal functions
t_word			*create_word_node(char *content, t_word_type type,
					bool expand_doller, bool expand_wildcard);
t_word			*append_node(t_word *head, t_word *new);

static t_word	*process_dollar_in_quote(const char *content, size_t *i,
		size_t len)
{
	size_t	start;
	char	*str;

	(*i)++;
	start = *i;
	if (content[*i] == '?')
		(*i)++;
	else
	{
		while (*i < len && (ft_isalnum(content[*i]) || content[*i] == '_'))
			(*i)++;
	}
	if (*i > start)
	{
		str = ft_strndup(&content[start], *i - start);
		if (!str)
			return (NULL);
		return (create_word_node(str, WD_DOLLER, true, false));
	}
	return (NULL);
}

static t_word	*process_literal_in_quote(const char *content, size_t *i,
		size_t len, bool is_double_quote)
{
	size_t	start;
	char	*str;

	start = *i;
	while (*i < len && !(is_double_quote && content[*i] == '$'))
		(*i)++;
	if (*i > start)
	{
		str = ft_strndup(&content[start], *i - start);
		if (!str)
			return (NULL);
		return (create_word_node(str, WD_LITERAL, false, false));
	}
	return (NULL);
}

static t_word	*process_content_segment(const char *content, size_t *i,
		size_t len, bool is_double_quote)
{
	if (is_double_quote && content[*i] == '$' && *i + 1 < len)
		return (process_dollar_in_quote(content, i, len));
	else
		return (process_literal_in_quote(content, i, len, is_double_quote));
}

t_word	*process_quoted_content(const char *content, size_t len,
		bool is_double_quote)
{
	t_word	*head;
	t_word	*word;
	size_t	i;

	head = NULL;
	i = 0;
	while (i < len)
	{
		word = process_content_segment(content, &i, len, is_double_quote);
		if (!word)
			return (free_word_list(head), NULL);
		head = append_node(head, word);
	}
	if (!head)
	{
		head = create_word_node(ft_strdup(""), WD_LITERAL, false, false);
		if (!head || !head->word)
			return (free_word_list(head), NULL);
	}
	return (head);
}

t_word	*handle_quoted_word(const char *input, size_t *idx, char quote)
{
	size_t	start;
	size_t	len;
	t_word	*words;

	start = *idx + 1;
	len = 0;
	while (input[start + len] && input[start + len] != quote)
		len++;
	if (input[start + len] != quote)
		return (NULL);
	*idx = start + len + 1;
	words = process_quoted_content(&input[start], len, (quote == '"'));
	return (words);
}
