/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_command_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:39 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 22:08:18 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token			*find_end_token(t_token *cur_token);
int				handle_redirection_parse(t_cmd *cmd, t_token *tmp);
int				process_token(t_cmd *cmd, t_token *tmp);

int	parse_redirection(t_redir **redir_head, t_token_type token_type,
		t_token *filename_token)
{
	t_redir	*new_redir;

	if (!redir_head || !filename_token)
		return (-1);
	new_redir = alloc_redir();
	if (!new_redir)
		return (-1);
	ft_memset(new_redir, 0, sizeof(t_redir));
	append_redir(redir_head, new_redir);
	set_redir_type(new_redir, token_type);
	new_redir->filename = ft_strdup(filename_token->value);
	if (!new_redir->filename)
		return (-1);
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (filename_token->in_dquote || filename_token->in_squote)
			new_redir->delim_quoted = true;
	}
	return (1);
}

int	parse_simple_command(t_cmd *cmd, t_token *command_token)
{
	t_token	**new_tokens;
	size_t	i;

	if (!cmd || !command_token)
		return (-1);
	new_tokens = xmalloc(sizeof(t_token *) * (cmd->token_count + 1));
	if (!new_tokens)
		return (-1);
	i = 0;
	while (i < cmd->token_count)
	{
		new_tokens[i] = cmd->tokens[i];
		i++;
	}
	new_tokens[cmd->token_count] = command_token;
	xfree(cmd->tokens);
	cmd->tokens = new_tokens;
	cmd->token_count++;
	return (1);
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

static t_ast	*init_command_node(t_ast *parent)
{
	t_ast	*node;

	node = alloc_node();
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_CMD;
	node->cmd = alloc_cmd();
	if (!node->cmd)
		return (xfree(node), NULL);
	ft_memset(node->cmd, 0, sizeof(t_cmd));
	return (node);
}

t_ast	*gen_command_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	int		result;

	if (!cur_token)
		return (NULL);
	node = init_command_node(parent);
	if (!node)
		return (NULL);
	result = parse_command_list(node->cmd, cur_token);
	if (result == -1)
		return (xfree(node->cmd), xfree(node), NULL);
	return (node);
}
