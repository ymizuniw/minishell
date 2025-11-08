
#include "../../includes/minishell.h"

static void	build_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;
	char	msg[100];
	int		len;
	t_ast	*right;

	if (!*cur_token || (*cur_token)->type == TK_EOF
		|| (*cur_token)->type == TK_NEWLINE || (*cur_token)->type == TK_LPAREN)
		return ;
	(void)msg;
	(void)len;
	node = NULL;
	if (token_is_operator((*cur_token)->type))
	{
		node = swap_with_parent(parent, cur_token);
		*parent = node;
		right = NULL;
		build_tree(&right, cur_token);
		node->right = right;
		return ;
	}
	else if (token_is_subshell_close((*cur_token)->type))
		node = gen_subshell_node(*parent, cur_token);
	else if (token_is_command((*cur_token)->type))
		node = gen_command_node(*parent, cur_token);
	if (node == NULL)
		return ;
	*parent = node;
	build_tree(parent, cur_token);
}

void	gen_tree(t_ast **parent, t_token **cur_token)
{
	build_tree(parent, cur_token);
}

void	gen_tree_subshell(t_ast **parent, t_token **cur_token)
{
	build_tree(parent, cur_token);
}

t_ast	*parser(t_token **cur_token)
{
	t_ast	*root;

	root = NULL;
	build_tree(&root, cur_token);
	return (root);
}
