#include "../../../includes/minishell.h"

static t_token_type	pipe_or_orif(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '|')
	{
		(*idx) += 2;
		return (TK_OR_IF);
	}
	else
	{
		(*idx) += 1;
		return (TK_PIPE);
	}
}

static t_token_type	redir_in_or_heredoc(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '<')
	{
		(*idx) += 2;
		return (TK_HEREDOC);
	}
	else
	{
		(*idx) += 1;
		return (TK_REDIRECT_IN);
	}
}

static t_token_type	redir_out_or_append(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '>')
	{
		(*idx) += 2;
		return (TK_APPEND);
	}
	else
	{
		(*idx) += 1;
		return (TK_REDIRECT_OUT);
	}
}

static t_token_type	andif_or_word(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '&')
	{
		(*idx) += 2;
		return (TK_AND_IF);
	}
	else
	{
		(*idx) += 1;
		return (TK_WORD);
	}
}

// get the token_type begin with the current idx of input.
t_token_type	get_token_type(char *input, size_t *idx)
{
	// printf("\n\nget_token_type is called\n\n");
	// printf("input[*idx]: %c\n", input[*idx]);
	if (input[*idx])
	{
		if (input[(*idx)] == '\n')
		{
			(*idx)++;
			return (TK_NEWLINE);
		}
		if (input[*idx] == '|')
			return (pipe_or_orif(input, idx));
		else if (input[*idx] == '<')
			return (redir_in_or_heredoc(input, idx));
		else if (input[*idx] == '>')
			return (redir_out_or_append(input, idx));
		else if (input[*idx] == '&')
			return (andif_or_word(input, idx));
		else if (input[*idx] == '(')
		{
			(*idx)++;
			return (TK_LPAREN);
		}
		else if (input[*idx] == ')')
		{
			(*idx)++;
			return (TK_RPAREN);
		}
		else if (input[*idx] == '$')
		{
			(*idx)++;
			return (TK_DOLLER);
		}
		else
			return (TK_WORD);
	}
	return (TK_EOF);
}
