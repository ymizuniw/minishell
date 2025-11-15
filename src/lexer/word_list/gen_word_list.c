#include "../../../includes/minishell.h"

// Forward declarations for internal word handlers
t_word	*handle_quoted_word(const char *input, size_t *idx, char quote);
t_word	*handle_unclosed_quote(const char *input, size_t input_len,
			size_t *idx);
t_word	*handle_doller_word(const char *input, size_t input_len, size_t *idx);
t_word	*handle_unquoted_word(const char *input, size_t input_len, size_t *idx);
t_word	*append_node(t_word *head, t_word *new);

static t_word	*get_word_by_type(const char *input, size_t input_len,
		size_t *idx, char quote)
{
	if (quote != '\0')
		return (handle_quoted_word(input, idx, quote));
	else if (input[*idx] == '$')
		return (handle_doller_word(input, input_len, idx));
	else
		return (handle_unquoted_word(input, input_len, idx));
}

t_word	*gen_word_list(const char *input, size_t input_len, size_t *idx)
{
	t_word	*word_list;
	t_word	*words;
	char	quote;

	word_list = NULL;
	while (*idx < input_len && !ft_isspace((unsigned char)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER)
	{
		quote = is_quote(input[*idx]);
		words = get_word_by_type(input, input_len, idx, quote);
		if (!words && quote != '\0')
			words = handle_unclosed_quote(input, input_len, idx);
		if (!words)
		{
			if (quote == '\0' && input[*idx] == '$')
				return (free_word_list(word_list), NULL);
			break ;
		}
		word_list = append_node(word_list, words);
	}
	return (word_list);
}
