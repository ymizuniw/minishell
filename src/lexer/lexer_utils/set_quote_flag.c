#include "../../../includes/minishell.h"

// set either double/single quotation flag to token.
void	set_quote_flag(t_token *token, char *input, char const quote_open)
{
	(void)input;  // Suppress unused parameter warning
	if (!token)
		return ;
	if (quote_open == '\"')
		token->in_dquote = true;
	else if (quote_open == '\'')
		token->in_squote = true;
}
