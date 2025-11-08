#include "../../includes/minishell.h"

static void	set_node_type(t_ast *node, t_token_type token_type)
{
	if (token_type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token_type == TK_OR_IF)
		node->type = NODE_OR;
	else if (token_type == TK_PIPE)
		node->type = NODE_PIPE;
}

t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;

	if (!parent || !cur_token || !*cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	set_node_type(node, (*cur_token)->type);
	if (*parent)
	{
		node->left = *parent;
		node->parent = (*parent)->parent;
		(*parent)->parent = node;
	}
	*cur_token = (*cur_token)->next;
	return (node);
}

static int	parse_subshell_redir(t_ast *node, t_token **redir_scan)
{
	while (*redir_scan && token_is_redirection((*redir_scan)->type))
	{
		if (!(*redir_scan)->prev)
			break ;
		if (!node->cmd)
		{
			node->cmd = alloc_cmd();
			if (!node->cmd)
				return (0);
			memset(node->cmd, 0, sizeof(t_cmd));
		}
		parse_redirection(&node->cmd->redir, (*redir_scan)->type,
			(*redir_scan)->prev);
		*redir_scan = (*redir_scan)->prev->prev;
	}
	return (1);
}

t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*subroot;
	t_token	*redir_scan;

	(void)parent;
	if (!cur_token || !*cur_token || (*cur_token)->type != TK_RPAREN)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_SUBSHELL;
	redir_scan = (*cur_token)->prev;
	parse_subshell_redir(node, &redir_scan);
	*cur_token = (*cur_token)->next;
	subroot = NULL;
	gen_tree_subshell(&subroot, cur_token);
	if (*cur_token && (*cur_token)->type == TK_LPAREN)
		*cur_token = (*cur_token)->next;
	node->subtree = subroot;
	return (node);
}

t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token)
{
	t_ast	*node;

	(void)parnt;
	(void)cur_token;
	node = NULL;
	return (node);
}
