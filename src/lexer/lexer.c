#include "../../includes/minishell.h"

int	init_token(t_token **token_head)
{
	*token_head = alloc_token();
	if (*token_head == NULL)
	{
		perror("token_alloc");
		return (0);
	}
	memset(*token_head, 0, sizeof(t_token));
	return (1);
}

// handle the newline token
int	handle_newline(t_token *token_head, const char *input, size_t *idx)
{
	t_token	*new;
	char	prev;

	if (*idx > 0)
	{
		prev = input[*idx - 1];
		if (prev == '|' || prev == '&' || prev == '(')
		{
			(*idx)++;
			return (1);
		}
	}
	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	new->type = TK_NEWLINE;
	new->value = strdup("\n");
	if (!new->value)
		return (-1);
	prepend_tokens(token_head, new);
	(*idx)++;
	return (1);
}

// handle meta_char token.
int	handle_meta_char(t_token *token_head, const char *input, size_t *idx)
{
	t_token	*new;

	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	new->type = get_token_type((char *)input, idx);
	prepend_tokens(token_head, new);
	return (1);
}

// handle doller token.
int	handle_doller(t_token *token_head, size_t *idx)
{
	t_token	*new;

	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	new->type = TK_DOLLER;
	new->value = strdup("$");
	if (!new->value)
		return (-1);
	prepend_tokens(token_head, new);
	(*idx)++;
	return (1);
}

// handle word token.
int	handle_word(t_token *token_head, char const *input, size_t input_len,
		size_t *idx)
{
	t_token	*new;
	char	*word;

	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	word = NULL;
	word_cat(&word, 0, (char *)input, input_len, idx);
	new->type = TK_WORD;
	new->value = word;
	new->next = NULL;
	prepend_tokens(token_head, new);
	return (1);
}

// entry point for handle_word/handle_doller token.
int	handle_word_and_doller(t_token *token_head, char const *input,
		size_t input_len, size_t *idx)
{
	if (is_doller_token(&input[*idx]))
	{
		if (handle_doller(token_head, idx) < 0)
			return (-1);
	}
	else
	{
		if (handle_word(token_head, input, input_len, idx) < 0)
			return (-1);
	}
	return (1);
}

// handle eof token.
int	handle_eof(t_token *token_head)
{
	t_token	*new;

	new = alloc_token();
	if (!new)
		return (-1);
	new->type = TK_EOF;
	new->value = strdup("");
	new->next = NULL;
	while (token_head->next)
		token_head = token_head->next;
	token_head->next = new;
	new->prev = token_head;
	return (1);
}

// handle newline and isspace charactors to skip them correctly.
int	handle_internal_separator(t_token *token_head, char const *input,
		size_t *idx)
{
	if (input[*idx] == '\n')
	{
		token_head->count_newline++;
		if (handle_newline(token_head, input, idx) < 0)
			return (-1);
	}
	if (input[*idx] && isspace((unsigned char)input[*idx]))
		(*idx)++;
	return (1);
}

// entry point for handle_operators/handle_words.
int	handle_operators_and_words(t_token *token_head, char const *input,
		size_t input_len, size_t *idx)
{
	t_metachar	meta;

	meta = is_meta_char(input[*idx]);
	if (meta != MT_OTHER)
	{
		if (handle_meta_char(token_head, input, idx) < 0)
			return (-1);
	}
	else
	{
		if (handle_word_and_doller(token_head, input, input_len, idx) < 0)
			return (-1);
	}
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
	input_len = strlen(input);
	while (idx < input_len)
	{
		if (isspace((unsigned char)input[idx])
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
