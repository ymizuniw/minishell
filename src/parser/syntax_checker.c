#include "../../includes/minishell.h"

//) command_list (
static int	check_lparen(t_token *token)
{
	(void)token;
	return (0);
}

// when this will be checked, the order is not reversed.
static int	check_redirection(t_token *token)
{
	t_token	*prev;

	if (token->prev)
		prev = token->prev;
	else
		return (0);
	if (token_is_command(prev->type))
		return (1);
	else
		return (0);
}

static int	check_operator(t_token *token)
{
	t_token	*next;
	t_token	*prev;

	if (token->prev && token->next)
	{
		prev = token->prev;
		next = token->next;
	}
	else
		return (0);
	if ((token_is_command(prev->type) || token_is_redirection(prev->type))
		&& (token_is_command(next->type) || token_is_redirection(next->type)))
		return (1);
	return (0);
}

// Entry: check the syntax of the given token is correct.
int	syntax_check(t_token *token)
{
	t_token_type	type;

	if (!token)
		return (0);
	type = token->type;
	if (type == TK_LPAREN)
		return (check_lparen(token));
	if (type == TK_RPAREN)
		return (check_parenthesis(token));
	if (token_is_redirection(type))
		return (check_redirection(token));
	if (token_is_operator(type))
		return (check_operator(token));
	return (1);
}
