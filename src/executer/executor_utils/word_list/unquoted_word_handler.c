/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquoted_word_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:18 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/25 23:06:43 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

// Forward declarations for internal functions
t_word		*create_word_node(char *content, t_word_type type,
				bool expand_doller, bool expand_wildcard);

static int	not_word(const char *input, size_t start, size_t len)
{
	if (ft_isspace((unsigned char)input[start + len]))
		return (1);
	if (is_meta_char(input[start + len]) != MT_OTHER)
		return (1);
	if (is_quote(input[start + len]))
		return (1);
	return (0);
}

t_word	*handle_unquoted_word(const char *input, size_t input_len, size_t *idx)
{
	size_t	start;
	size_t	len;
	char	*content;
	bool	has_wildcard;

	start = *idx;
	len = 0;
	has_wildcard = false;
	while (start + len < input_len)
	{
		if (not_word(input, start, len))
			break ;
		if (input[start + len] == '*')
			has_wildcard = true;
		if (input[start + len] == '$')
			break ;
		len++;
	}
	if (len == 0)
		return (NULL);
	content = ft_strndup(&input[start], len);
	if (!content)
		return (NULL);
	*idx = start + len;
	return (create_word_node(content, WD_LITERAL, false, has_wildcard));
}

t_word	*handle_unclosed_quote(const char *input, size_t input_len, size_t *idx)
{
	size_t	start;
	size_t	len;
	char	*content;

	start = *idx;
	len = 0;
	while (start + len < input_len)
	{
		if (ft_isspace((unsigned char)input[start + len])
			|| is_meta_char(input[start + len]) != MT_OTHER)
			break ;
		if (input[start + len] == '$')
			break ;
		len++;
	}
	if (len == 0)
		return (NULL);
	content = ft_strndup(&input[start], len);
	if (!content)
		return (NULL);
	*idx = start + len;
	return (create_word_node(content, WD_LITERAL, false, false));
}

t_word	*handle_doller_word(const char *input, size_t input_len, size_t *idx)
{
	size_t	start;
	size_t	len;
	char	*content;

	if (input[*idx] != '$')
		return (NULL);
	start = *idx + 1;
	len = 0;
	if (input[start] == '?')
		len = 1;
	else
	{
		while (start + len < input_len && (ft_isalnum(input[start + len])
				|| input[start + len] == '_'))
			len++;
	}
	if (len == 0)
		return (NULL);
	content = ft_strndup(&input[start], len);
	if (!content)
		return (NULL);
	*idx = start + len;
	return (create_word_node(content, WD_DOLLER, true, false));
}
