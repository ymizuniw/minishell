#include "../../../includes/minishell.h"

t_word	*handle_unquoted_word(const char *input, size_t input_len, size_t *idx)
{
	size_t	start;
	size_t	len;
	char	*content;
	bool	has_wildcard;

	start = *idx;
	len = 0;
	has_wildcard = false;
	while (start + len < input_len && !ft_isspace((unsigned char)input[start
			+ len]) && is_meta_char(input[start + len]) == MT_OTHER
		&& !is_quote(input[start + len]))
	{
		if (input[start + len] == '*')
			has_wildcard = true;
		if (input[start + len] == '$')
			break ;
		len++;
	}
	if (len == 0)
		return (NULL);
	content = strndup(&input[start], len);
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
	while (start + len < input_len && !ft_isspace((unsigned char)input[start
			+ len]) && is_meta_char(input[start + len]) == MT_OTHER)
	{
		if (input[start + len] == '$')
			break ;
		len++;
	}
	if (len == 0)
		return (NULL);
	content = strndup(&input[start], len);
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
	content = strndup(&input[start], len);
	if (!content)
		return (NULL);
	*idx = start + len;
	return (create_word_node(content, WD_DOLLER, true, false));
}
