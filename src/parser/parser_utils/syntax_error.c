#include "../../../includes/minishell.h"

void	syntax_error(t_token_type type)
{
	char	*token = "unknown";
	if (type == TK_NEWLINE)
		token = TOKEN_VALUE_NEWLINE;
	else if (type == TK_PIPE)
		token = TOKEN_VALUE_PIPE;
	else if (type == TK_AND_IF)
		token = TOKEN_VALUE_AND_IF;
	else if (type == TK_OR_IF)
		token = TOKEN_VALUE_OR_IF;
	else if (type == TK_LPAREN)
		token = TOKEN_VALUE_LPAREN;
	else if (type == TK_RPAREN)
		token = TOKEN_VALUE_RPAREN;
	else if (type == TK_REDIRECT_IN)
		token = TOKEN_VALUE_REDIRECT_IN;
	else if (type == TK_REDIRECT_OUT)
		token = TOKEN_VALUE_REDIRECT_OUT;
	else if (type == TK_HEREDOC)
		token = TOKEN_VALUE_HEREDOC;
	else if (type == TK_APPEND)
		token = TOKEN_VALUE_APPEND;
	write(2, SYNTAX_ERR_TEMPLATE, SYNTAX_ERR_TEMPLATE_LEN);
	write(2, token, strlen(token));
	write(2, "`\n", 2);
}
