#include "../../includes/minishell.h"

int init_token(t_token **token_head)
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

int handle_newline(t_token *token_head, const char *input, size_t *idx)
{
	t_token		*new;

	if (*idx>0)
	{
		char prev = input[*idx-1];
		if (prev=='|' || prev == '&' || prev == '(')
		{
			*idx++;
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
	append_tokens(token_head, new);
	*idx++;
	return (1);
}

int handle_meta_char(t_token *token_head, const char *input, size_t *idx)
{
	t_token *new;
	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	new->type = get_token_type((char *)input, idx);
	append_tokens(token_head, new);
	return (1);
}

int handle_doller(t_token *token_head, char const *input, size_t *idx)
{
	t_token *new;
	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	new->type = TK_DOLLER;
	new->value = strdup("$");
	if (!new->value)
		return (-1);
	append_tokens(token_head, new);
	*idx++;
	return (1);
}

int handle_word(t_token *token_head, char const *input, size_t input_len, size_t *idx)
{
	t_token *new;
	new = alloc_token();
	if (!new)
		return (-1);
	memset(new, 0, sizeof(t_token));
	
	char	*word;
	size_t consumed = 0;
	word = NULL;
	consumed = word_cat(&word, 0, (char *)input, input_len, *idx);
	if (consumed == 0)
	{
		xfree(new);
		free_token_list(token_head);
		return (-1);
	}
	// if (is_quote(input[idx]))
	// 	set_quote_flag(new, (char *)input, is_quote(input[idx]));
	*idx += consumed;
	new->type = TK_WORD;
	new->value = word;
	new->next = NULL;
	append_tokens(token_head, new);	//prepend_tokens is better naming, maybe.
	return (1);
}

int handle_word_and_doller(t_token *token_head,char const *input,  size_t input_len, size_t *idx)
{
	if (is_doller_token(&input[*idx]))
	{
		if (handle_doller(token_head, input, idx)<0)
			return (-1);
	}
	else
	{
		if (handle_word(token_head, input, input_len, idx)<0)
			return (-1);
	}
	return (1);
}

int handle_eof(t_token *token_head)
{
	t_token *new = alloc_token();
	if (!new)
		return (-1);
	new->type = TK_EOF;
	if (!new->value)
		new->value = strdup("");
	new->next = NULL;
	while (token_head->next)
		token_head = token_head->next;
	token_head->next = new;
	new->prev = token_head;
	return (1);
}

int handle_internal_separator(t_token *token_head, char const *input, size_t *idx)
{
	if (input[*idx]=='\n')
	{
		token_head->count_newline++;
		if (handle_newline(token_head, input, idx)<0)
			return (-1);
	}
	if (input[*idx] && isspace((unsigned char)input[*idx]))
		*idx++;
	return (1);	
}

int handle_operators_and_words(t_token *token_head, char const *input, size_t input_len, size_t *idx)
{
	t_metachar	meta = is_meta_char(input[*idx]);
	if (meta != MT_OTHER)
	{
		if (handle_meta_char(token_head, input, idx)<0)
			return (-1);
	}
	else
	{
		if (handle_word_and_doller(token_head, input_len, input, idx)<0)
			return (-1);
	}
	return (1);
}

t_token	*lexer(const char *input)
{
	size_t		idx;
	size_t		input_len;
	t_token 	*dummy_head;
	t_token		*token_head;
	t_metachar	meta;
	size_t		consumed;

	if (init_token(&dummy_head)<0)
		return (NULL);
	dummy_head->type = TK_HEAD;
	idx = 0;
	input_len = strlen(input);
	while (idx < input_len)
	{
		if (handle_internal_separator(dummy_head, input, &idx)<0)
			return (NULL);
		if (idx >= input_len)
			break ;
		if (handle_operators_and_words(dummy_head, input, input_len, &idx)<0)
			return (NULL);
	}
	if (handle_eof(dummy_head)<0)
		return(NULL);
	return (dummy_head);
}
