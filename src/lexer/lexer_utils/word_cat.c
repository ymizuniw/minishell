#include "../../../includes/minishell.h"

int	handle_quotation(char **word, size_t word_len, char const *input,
		size_t *idx, size_t *consumed_len, char quote_open)
{
	size_t	add_len;
	char	*quote_close;

	quote_close = NULL;
	if (input[*idx + 1])
		quote_close = strchr(&input[*idx + 1], quote_open);
	if (!quote_close)
		return (0);
	add_len = quote_close - &input[*idx] - 1;
	*word = realloc(*word, sizeof(char) * (word_len + add_len + 1));
	if (!*word)
		return (-1);
	strncpy(*word + word_len, &input[*idx + 1], add_len);
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
	while (*idx < input_len && !isspace((int)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER)
		(*idx)++;
	add_len = &input[*idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*word = realloc(*word, sizeof(char) * (*word_len + (add_len) + 1));
	if (!*word)
		return (-1);
	strncpy(*word + *word_len, tmp_ptr, add_len);
	(*word)[*word_len + add_len] = '\0';
	*word_len += add_len;
	return (1);
}

size_t	word_cat(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx, bool *had_sq, bool *had_dq,
		bool *had_unq)
{
	char	q_open;
	char	*d_close;
	size_t	ext_len;

	// printf("\n==word_cat() called.==\ninput[%zu]: %s\n", *idx, &input[*idx]);
	while (*idx < input_len && !isspace((int)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER)
	{
		q_open = is_quote(input[*idx]);
		if (q_open != '\0')
		{
			d_close = strchr(&input[*idx + 1], q_open);
			if (d_close)
			{
				ext_len = (size_t)(d_close - &input[*idx + 1]);
				*word = realloc(*word, sizeof(char) * (word_len + ext_len + 1));
				if (!*word)
					return (0);
				memcpy(*word + word_len, &input[*idx + 1], ext_len);
				(*word)[word_len + ext_len] = '\0';
				*idx += ext_len + 2;
				if (q_open == '\'')
					*had_sq = true;
				else if (q_open == '"')
					*had_dq = true;
				word_len += ext_len;
				continue ;
			}
		}
		if (handle_plain(word, &word_len, input, input_len, idx) < 0)
			return (0);
		*had_unq = true;
	}
	return (1);
}
