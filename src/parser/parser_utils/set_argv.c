#include "../../../includes/minishell.h"

// expand the value of the token and set to cmd list's argv.
void	set_argv(char **argv, t_token **start_token, size_t i)
{
	t_token	*token;
	char	**new_argv;

	if (!argv || !start_token || !*start_token)
		return ;
	token = *start_token;
	if (token->type != TK_WORD && token->type != TK_DOLLER)
		return ;
	new_argv = realloc(argv, sizeof(char *) * (i + 2));
	if (new_argv == NULL)
	{
		perror("realloc");
		return ;
	}
	argv = new_argv;
	if (token->type == TK_DOLLER)
		argv[i] = strdup("$");
	else
		argv[i] = expand_value(token);
	argv[i + 1] = NULL;
	if (token->next && token->next->type != TK_REDIRECT_IN
		&& token->next->type != TK_REDIRECT_OUT
		&& token->next->type != TK_HEREDOC && token->next->type != TK_APPEND
		&& (token->next->type == TK_WORD || token->next->type == TK_DOLLER))
	{
		*start_token = token->next;
		set_argv(argv, start_token, i + 1);
	}
}
