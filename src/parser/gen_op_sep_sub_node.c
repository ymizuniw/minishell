#include "../../includes/minishell.h"

t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;

	if (!parent || !cur_token || !*cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	if ((*cur_token)->type == TK_AND_IF)
		node->type = NODE_AND;
	else if ((*cur_token)->type == TK_OR_IF)
		node->type = NODE_OR;
	else if ((*cur_token)->type == TK_PIPE)
		node->type = NODE_PIPE;
	if (*parent)
	{
		node->left = *parent;
		node->parent = (*parent)->parent;
		(*parent)->parent = node;
	}
	*cur_token = (*cur_token)->next;
	return (node);
}

t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*subroot;

	if (!cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_SUBSHELL;
	subroot = NULL;
	// Skip TK_RPAREN first (marks beginning in reversed order)
	if ((*cur_token)->type == TK_RPAREN)
		*cur_token = (*cur_token)->next;
	// Parse until TK_LPAREN (marks end in reversed order)
	while (*cur_token && (*cur_token)->type != TK_LPAREN
		&& (*cur_token)->type != TK_EOF)
	{
		fgen_tree(&subroot, cur_token);
		if (*cur_token && (*cur_token)->type == TK_LPAREN)
			break ;
	}
	// Skip TK_LPAREN at end
	if (*cur_token && (*cur_token)->type == TK_LPAREN)
		*cur_token = (*cur_token)->next;
	node->subtree = subroot;
	return (node);
}

// end of command or command line.
t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token)
{
	t_ast	*node;

	(void)parnt;
	(void)cur_token;
	node = NULL; // this can be descriminated from syntax error node NULL?
	return (node);
}
