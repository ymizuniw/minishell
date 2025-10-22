#include "../../../includes/minishell.h"

// create a list of redirection. tail_token is cpy of tail_token of caller.
t_redir	*parse_redirection(t_redir *redir, t_token **tail_token)
{
	t_redir	*cur_redir;
	t_redir	*head;
	t_token	*token;
	char	*file_name;
	t_token	*next;

	if (!tail_token || !*tail_token)
		return (redir);
	token = *tail_token;
	next = token->next;
	if (!next || (next->type != TK_REDIRECT_IN && next->type != TK_REDIRECT_OUT
			&& next->type != TK_HEREDOC && next->type != TK_APPEND))
		return (redir);
	cur_redir = alloc_redir();
	if (!cur_redir)
		return (redir);
	bzero(cur_redir, sizeof(t_redir));
	if (syntax_check(next) != 1)
	{
		free(cur_redir);
		return (NULL);
	}
	if (next->type == TK_REDIRECT_IN)
	{
		if (next->next == NULL)
		{
			free(cur_redir);
			return (NULL);
		}
		file_name = expand_value(next->next);
		cur_redir->type = REDIR_IN;
		cur_redir->filename = file_name;
	}
	else if (next->type == TK_REDIRECT_OUT)
	{
		if (next->next == NULL)
		{
			free(cur_redir);
			return (NULL);
		}
		file_name = expand_value(next->next);
		cur_redir->type = REDIR_OUT;
		cur_redir->filename = file_name;
	}
	else if (next->type == TK_HEREDOC)
	{
		if (next->next == NULL)
		{
			free(cur_redir);
			return (NULL);
		}
		cur_redir->type = REDIR_HEREDOC;
		cur_redir->filename = next->next->value;
		if (next->next->in_dquote || next->next->in_squote)
			cur_redir->delim_quoted = true;
	}
	else if (next->type == TK_APPEND)
	{
		if (next->next == NULL)
		{
			free(cur_redir);
			return (NULL);
		}
		file_name = expand_value(next->next);
		cur_redir->type = REDIR_APPEND;
		cur_redir->filename = file_name;
	}
	if (redir == NULL)
		head = cur_redir;
	else
	{
		redir->next = cur_redir;
		head = redir;
	}
	*tail_token = next->next->next;
	if (*tail_token && ((*tail_token)->type == TK_WORD
			|| (*tail_token)->type == TK_DOLLER))
		return (parse_redirection(head, tail_token));
	return (head);
}
