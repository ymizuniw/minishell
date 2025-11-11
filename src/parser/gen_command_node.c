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
	redir_add_back(redir_head, new_redir);
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

// generate command's word_list from command_token.
int	parse_simple_command(t_word **word_list, t_token *command_token)
{
	t_word	*new_argv;
	size_t	addition;
	size_t	value_len;
	t_word	*w;

	if (!word_list || !command_token)
		return (-1);
	addition = 0;
	value_len = 0;
	if (command_token->value)
		value_len = strlen(command_token->value);
	new_argv = gen_word(command_token->value, value_len, &addition);
	// If token is in single quotes, disable dollar expansion
	if (command_token->in_squote && new_argv)
	{
		w = new_argv;
		while (w)
		{
			w->to_expand_doller = false;
			w = w->next;
		}
	}
	// Append to the end of the word list instead of overwriting
	word_add_back(word_list, new_argv);
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
		return (parse_simple_command(&cmd->word_list, tmp));
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
		xfree(node);
		return (NULL);
	}
	memset(node->cmd, 0, sizeof(t_cmd));
	result = parse_command_list(node->cmd, cur_token);
	if (result == -1)
	{
		xfree(node->cmd);
		xfree(node);
		return (NULL);
	}
	return (node);
}
