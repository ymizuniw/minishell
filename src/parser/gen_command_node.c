#include "../../includes/minishell.h"

static void	set_redir_type(t_redir *new_redir, t_token_type token_type)
{
	if (token_type == TK_REDIRECT_IN)
		new_redir->type = REDIR_IN;
	else if (token_type == TK_REDIRECT_OUT)
		new_redir->type = REDIR_OUT;
	else if (token_type == TK_HEREDOC)
		new_redir->type = REDIR_HEREDOC;
	else
		new_redir->type = REDIR_APPEND;
}

static void	append_redir(t_redir **redir_head, t_redir *new_redir)
{
	t_redir	*redir;

	if (*redir_head == NULL)
		*redir_head = new_redir;
	else
	{
		redir = *redir_head;
		while (redir->next)
			redir = redir->next;
		redir->next = new_redir;
	}
}

int	parse_redirection(t_redir **redir_head, t_token_type token_type,
		t_token *filename_token)
{
	t_redir	*new_redir;

	if (!redir_head || !filename_token)
		return (-1);
	new_redir = alloc_redir();
	if (!new_redir)
		return (-1);
	memset(new_redir, 0, sizeof(t_redir));
	append_redir(redir_head, new_redir);
	set_redir_type(new_redir, token_type);
	new_redir->filename = strdup(filename_token->value);
	if (!new_redir->filename)
		return (-1);
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (filename_token->in_dquote || filename_token->in_squote)
			new_redir->delim_quoted = true;
	}
	return (1);
}

int	parse_simple_command(t_argv **argv_head, t_token *command_token)
{
	t_argv	*new_argv;

	if (!argv_head || !command_token)
		return (-1);
	new_argv = alloc_argv();
	if (!new_argv)
		return (-1);
	memset(new_argv, 0, sizeof(t_argv));
	new_argv->word = strdup(command_token->value);
	if (!new_argv->word)
		return (free(new_argv), -1);
	if (command_token->in_squote == false && strchr(command_token->value, '$'))
		new_argv->to_expand = true;
	else
		new_argv->to_expand = false;
	new_argv->next = *argv_head;
	*argv_head = new_argv;
	return (1);
}

static t_token	*find_end_token(t_token *cur_token)
{
	t_token	*tmp;

	tmp = cur_token;
	while (tmp->next && (token_is_command(tmp->next->type)
			|| token_is_redirection(tmp->next->type)))
		tmp = tmp->next;
	return (tmp->next);
}

static int	handle_redirection_parse(t_cmd *cmd, t_token *tmp)
{
	if (!syntax_check(tmp))
		return (syntax_error(tmp->type), -1);
	if (!tmp->next)
		return (-1);
	if (!tmp->prev)
		return (write(2,
				"minishell: syntax error: missing redirection target\n", 53),
			-1);
	return (parse_redirection(&cmd->redir, tmp->type, tmp->prev));
}

static int	process_token(t_cmd *cmd, t_token *tmp)
{
	if (token_is_redirection(tmp->type))
		return (handle_redirection_parse(cmd, tmp));
	else
		return (parse_simple_command(&cmd->argv_list, tmp));
}

int	parse_command_list(t_cmd *cmd, t_token **cur_token)
{
	t_token	*tmp;
	t_token	*end_token;
	int		parse_success;

	if (!cmd || !cur_token || !*cur_token)
		return (-1);
	end_token = find_end_token(*cur_token);
	tmp = end_token->prev;
	while (tmp && (token_is_command(tmp->type)
			|| token_is_redirection(tmp->type)))
	{
		parse_success = process_token(cmd, tmp);
		if (parse_success <= 0)
			return (printf("parse %s failed\n", tmp->value), -1);
		if (token_is_redirection(tmp->type))
			tmp = tmp->prev;
		tmp = tmp->prev;
	}
	*cur_token = end_token;
	return (1);
}

t_ast	*gen_command_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	int		result;

	if (!cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_CMD;
	node->cmd = alloc_cmd();
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	memset(node->cmd, 0, sizeof(t_cmd));
	result = parse_command_list(node->cmd, cur_token);
	if (result == -1)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	return (node);
}
