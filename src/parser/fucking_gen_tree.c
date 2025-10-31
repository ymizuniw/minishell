#include "../../includes/minishell.h"

// syntax_check is implemented in syntax_checker.c

void	fgen_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*op_node;

	if (!parent || !cur_token || !*cur_token || (*cur_token)->type == TK_HEAD)
		return ;
	// Stop at NEWLINE or EOF (command separator)
	if (token_is_newline_or_eof((*cur_token)->type))
	{
		// Advance past NEWLINE for next command
		if ((*cur_token)->type == TK_NEWLINE)
			*cur_token = (*cur_token)->next;
		return ;
	}
	node = NULL;
	if (token_is_command((*cur_token)->type))
		node = gen_command_node(*parent, cur_token);
	else if (token_is_subshell_close((*cur_token)->type))
		node = gen_subshell_node(*parent, cur_token);
	else
		node = gen_eof_newline_node(*parent, cur_token);
	if (!node)
		return ;
	if (*cur_token && token_is_operator((*cur_token)->type))
	{
		op_node = swap_with_parent(&node, cur_token);
		if (!op_node)
		{
			*parent = node;
			return ;
		}
		fgen_tree(&op_node->right, cur_token);
		if (op_node->right)
			op_node->right->parent = op_node;
		*parent = op_node;
	}
	else
		*parent = node;
}

t_ast	*parse(t_token **cur_token)
{
	t_ast	*root;

	root = NULL;
	if (!cur_token || !*cur_token)
		return (NULL);
	// Skip TK_HEAD if present (it's just a sentinel node)
	if ((*cur_token)->type == TK_HEAD)
		*cur_token = (*cur_token)->next;
	if (!*cur_token || (*cur_token)->type == TK_EOF)
		return (NULL);
	fgen_tree(&root, cur_token);
	return (root);
}

// legacy entry point expected by main.c and headers
t_ast	*parser(t_token **cur_token)
{
	return (parse(cur_token));
}
