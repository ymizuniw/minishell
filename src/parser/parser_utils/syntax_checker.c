#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF)
		return (1);
	return (0);
}

		int check_lparen(t_token *token)
		{
			if (token->prev && (token->prev->type == TK_WORD || token->prev->type == TK_DOLLER))
			{
				syntax_error(token->type);
				return (0);
			}
			return (1);
		}

		int check_redirection(t_token *token)
		{
			if (token->prev != NULL && (token->prev->type != TK_WORD && token->prev->type != TK_DOLLER))
			{
				syntax_error(token->type);
				return (0);
			}
			return (1);
		}

int check_operator(t_token *token)
{
	//operator permits
	// word && word
	// word && redirection
	// word && l_paren
	// r_paren && word
	// r_paren && redirection
	// r_paren && l_paren
	if ((token->next && token->next->type != TK_WORD && token->next->type != TK_RPAREN) || (token->prev && token->prev->type != TK_WORD && token->prev->type != TK_REDIRECT_IN && token->prev->type != TK_REDIRECT_OUT && token->prev->type != TK_HEREDOC && token->prev->type != TK_APPEND && token->prev->type != TK_LPAREN))
		return (0);
	return (1);
}

// check the syntax of given token is correct.
int	syntax_check(t_token *token)
{
	t_token_type	token_type;

	if (!token || !token->prev)
		return (0);
	token_type = token->type;
	if (token_type == TK_LPAREN)
		return (check_lparen(token));
	if (token_type == TK_RPAREN)
		return (check_parenthesis(token));
	if (token_type == TK_REDIRECT_IN || token_type == TK_HEREDOC || token_type == TK_REDIRECT_OUT || token_type == TK_APPEND)
		return (check_redirection(token));
	if (is_operator(token->type))
		return (check_operator(token));
	return (1);
}
