#include "../../../includes/minishell.h"

int	handle_quotation(char **word, size_t word_len, char const *input,
		size_t *idx, size_t *consumed_len, char quote_open)
{
	size_t	add_len;
	char	*quote_close;

	quote_close = NULL;
	if (input[*idx + 1])
		quote_close = ft_strchr(&input[*idx + 1], quote_open);
	if (!quote_close)
		return (0);
	add_len = quote_close - &input[*idx] - 1;
	*word = ft_realloc(*word, sizeof(char) * (word_len + 1), sizeof(char)
			* (word_len + add_len + 1));
	if (!*word)
		return (-1);
	ft_memcpy(*word + word_len, &input[*idx + 1], add_len);
	(*word)[add_len] = '\0';
	*idx += add_len + 2;
	*consumed_len = add_len;
	return (1);
}

int	handle_plain(char **word, size_t *word_len, char const *input,
		size_t input_len, size_t *idx)
{
	size_t		add_len;
	const char	*tmp_ptr;

	add_len = 0;
	tmp_ptr = &input[*idx];
	while (*idx < input_len && !ft_isspace((unsigned char)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER)
		(*idx)++;
	add_len = &input[*idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*word = ft_realloc(*word, sizeof(char) * (*word_len + 1), sizeof(char)
			* (*word_len + (add_len) + 1));
	if (!*word)
		return (-1);
	ft_memcpy(*word + *word_len, tmp_ptr, add_len);
	(*word)[*word_len + add_len] = '\0';
	*word_len += add_len;
	return (1);
}

size_t	word_cat(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx)
{
	char	quote;
	size_t	start;

	start = *idx;
	quote = 0;
	while (*idx < input_len)
	{
		if (!quote && is_quote(input[*idx]))
			quote = input[*idx];
		else if (quote && input[*idx] == quote)
			quote = 0;
		else if (!quote && (ft_isspace((unsigned char)input[*idx])
				|| is_meta_char(input[*idx]) != MT_OTHER))
			break ;
		(*idx)++;
	}
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		return (0);
	}
	*word = ft_realloc(*word, sizeof(char) * (word_len + 1),
			sizeof(char) * (word_len + (*idx - start) + 1));
	if (!*word)
		return (0);
	ft_memcpy(*word + word_len, &input[start], *idx - start);
	(*word)[word_len + (*idx - start)] = '\0';
	return (1);
}
