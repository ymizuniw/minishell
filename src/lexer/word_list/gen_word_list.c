#include "../../../includes/minishell.h"

t_word	*gen_word_list(const char *input, size_t input_len, size_t *idx)
{
	t_word	*word_list;
	t_word	*words;
	char	quote;

	word_list = NULL;
	while (*idx < input_len && !isspace((int)input[*idx])
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
