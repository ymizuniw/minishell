#include "../../includes/minishell.h"

int	token_is_operator(t_token_type type)
{
	if (type == TK_AND_IF || type == TK_OR_IF || type == TK_PIPE)
		return (1);
	return (0);
}

int	token_is_subshell_close(t_token_type type)
{
	if (type == TK_LPAREN)
		return (1);
	return (0);
}

int	token_is_command(t_token_type type)
{
	if (type == TK_WORD || type == TK_DOLLER)
		return (1);
	return (0);
}

int	token_is_redirection(t_token_type type)
{
	if (type == TK_REDIRECT_IN || type == TK_REDIRECT_OUT || type == TK_HEREDOC
		|| type == TK_APPEND)
		return (1);
	return (0);
}

int	token_is_newline_or_eof(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_EOF)
		return (1);
	return (0);
}
