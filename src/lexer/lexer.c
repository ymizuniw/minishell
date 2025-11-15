#include "../../includes/minishell.h"

int	handle_eof(t_token *token_head);
int	handle_internal_separator(t_token *token_head, char const *input,
		size_t *idx);
int	handle_operators_and_words(t_token *token_head, char const *input,
		size_t input_len, size_t *idx);

int	init_token(t_token **token_head)
{
	*token_head = alloc_token();
	if (*token_head == NULL)
	{
		perror("token_alloc");
		return (0);
	}
	ft_memset(*token_head, 0, sizeof(t_token));
	return (1);
}

t_token	*lexer(const char *input)
{
	size_t	idx;
	size_t	input_len;
	t_token	*dummy_head;

	if (init_token(&dummy_head) < 0)
		return (NULL);
	dummy_head->type = TK_HEAD;
	idx = 0;
	input_len = 0;
	if (input)
		input_len = ft_strlen(input);
	while (idx < input_len)
	{
		if (input[idx] && ft_isspace((unsigned char)input[idx])
			&& handle_internal_separator(dummy_head, input, &idx) < 0)
			return (NULL);
		if (idx >= input_len)
			break ;
		if (handle_operators_and_words(dummy_head, input, input_len, &idx) < 0)
			return (NULL);
	}
	if (handle_eof(dummy_head) < 0)
		return (NULL);
	return (dummy_head);
}
