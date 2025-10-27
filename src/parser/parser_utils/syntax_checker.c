#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF)
		return (1);
	return (0);
}

// check the syntax of given token is correct.
int	syntax_check(t_token *token)
{
	t_token_type	token_type;

	if (!token || !token->prev)
		return (0);
	token_type = token->type;

	// ( left parenthesis
	if (token_type == TK_LPAREN)
	{
		if (token->prev && (token->prev->type == TK_WORD || token->prev->type == TK_DOLLER))
		{
			syntax_error(token->type);
			return (0);
		}
		return (1);
	}
	// ) right parenthesis
	if (token_type == TK_RPAREN)
		return (check_parenthesis(token));

	// <
	if (token_type == TK_REDIRECT_IN)
	{
		if (token->prev->type == TK_NEWLINE)
		{
			syntax_error(TK_NEWLINE);
			return (0);
		}
		if (!token->next || token->next->type == TK_EOF
			|| is_operator(token->next->type))
		{
			syntax_error(token->type);
			return (0);
		}
	}
	if (token_type == TK_HEREDOC)
	{
		if (token->prev->type != TK_WORD && token->prev->type != TK_NEWLINE
			&& token->prev->type != TK_EOF && token->prev->type != TK_AND_IF
			&& token->prev->type != TK_OR_IF && token->prev->type != TK_PIPE)
		{
			syntax_error(token->prev->type);
			return (0);
		}
		if (!token->next || token->next->type == TK_EOF
			|| is_operator(token->next->type))
		{
			syntax_error(token->type);
			return (0);
		}
	}
	// >, >>, <>
	if (token_type == TK_REDIRECT_OUT || token_type == TK_APPEND)
	{
		if (token->prev->type != TK_WORD && token->prev->type != TK_NEWLINE
			&& token->prev->type != TK_EOF && token->prev->type != TK_AND_IF
			&& token->prev->type != TK_OR_IF && token->prev->type != TK_PIPE)
		{
			syntax_error(token->prev->type);
			return (0);
		}
		if (!token->next || token->next->type == TK_EOF
			|| is_operator(token->next->type))
		{
			syntax_error(token->type);
			return (0);
		}
	}
	return (1);
}
