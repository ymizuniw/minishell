#include "../../includes/minishell.h"

t_ast	*handle_operator(t_ast **parent, t_token **cur_token);

void	build_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;

	if (!*cur_token || (*cur_token)->type == TK_EOF
		|| (*cur_token)->type == TK_LPAREN)
		return ;
	node = NULL;
	if (token_is_operator((*cur_token)->type))
	{
		*parent = handle_operator(parent, cur_token);
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
