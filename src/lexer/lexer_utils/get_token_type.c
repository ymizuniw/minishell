#include "../../../includes/minishell.h"

// get the token_type begin with the current idx of input.
t_token_type	get_token_type(char *input, size_t *idx)
{
	if (input[*idx])
	{
		if (input[*idx] == '|')
		{
			if (input[*idx + 1] && input[*idx + 1] == '|')
			{
				++*idx;
				return (TK_OR_IF);
			}
			else
				return (TK_PIPE);
		}
		else if (input[*idx] == '<')
		{
			if (input[*idx + 1] && input[*idx + 1] == '<')
			{
				++*idx;
				return (TK_HEREDOC);
			}
			else if (input[*idx + 1] && input[*idx + 1] == '>')
			{
				++*idx;
				return (TK_REDIRECT_IN_AND_OUT);
			}
			else
				return (TK_REDIRECT_IN);
		}
		else if (input[*idx] == '>')
		{
			if (input[*idx + 1] && input[*idx + 1] == '>')
			{
				++*idx;
				return (TK_APPEND);
			}
			else
				return (TK_REDIRECT_OUT);
		}
		else if (input[*idx] == '&')
		{
			if (input[*idx + 1] && input[*idx + 1] == '&')
			{
				++*idx;
				return (TK_AND_IF);
			}
			else
				return (TK_WORD); // Single & treated as word
		}
		else if (input[*idx] == '(')
		{
			return (TK_LPAREN);
		}
		else if (input[*idx] == ')')
		{
			return (TK_RPAREN);
		}
		else if (input[*idx] == '$') //$"word", $<EOF>, $<NEWLINE>,
			// '$' print token.
			{
				return (TK_DOLLER);
			}
		else
			return (TK_WORD);
	}
	return (TK_EOF);
}
