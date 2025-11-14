#include "../../../includes/minishell.h"

// Forward declarations for internal word handlers
t_word	*handle_quoted_word(const char *input, size_t *idx, char quote);
t_word	*handle_unclosed_quote(const char *input, size_t input_len,
			size_t *idx);
t_word	*handle_doller_word(const char *input, size_t input_len, size_t *idx);
t_word	*handle_unquoted_word(const char *input, size_t input_len, size_t *idx);
t_word	*append_node(t_word *head, t_word *new);

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
		if (quote != '\0')
		{
			words = handle_quoted_word(input, idx, quote);
			if (!words)
				words = handle_unclosed_quote(input, input_len, idx);
			if (!words)
				break ;
			word_list = append_node(word_list, words);
		}
		else if (input[*idx] == '$')
		{
			words = handle_doller_word(input, input_len, idx);
			if (!words)
				return (free_word_list(word_list), NULL);
			word_list = append_node(word_list, words);
		}
		else
		{
			words = handle_unquoted_word(input, input_len, idx);
			if (!words)
				break ;
			word_list = append_node(word_list, words);
		}
	}
	return (word_list);
}
