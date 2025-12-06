/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_quoted_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:39:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:41:19 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_word	*process_dollar_in_quote(const char *content, size_t *i, size_t len)
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
		return (create_word_node(str, WD_DOLLER, true));
	}
	return (NULL);
}

t_word	*process_literal_in_quote(const char *content, size_t *i, size_t len,
		bool is_double_quote)
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
		return (create_word_node(str, WD_LITERAL, false));
	}
	return (NULL);
}

t_word	*process_content_segment(const char *content, size_t *i, size_t len,
		bool is_double_quote)
{
	if (is_double_quote && content[*i] == '$' && *i + 1 < len)
		return (process_dollar_in_quote(content, i, len));
	else
		return (process_literal_in_quote(content, i, len, is_double_quote));
}
