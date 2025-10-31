#include "../../../../includes/minishell.h"

/*
** ast_traversal: Recursively execute an AST tree
**
** IMPORTANT: The parser builds operators with REVERSED left/right structure:
**   - For "a && b", tree is AND(left=b, right=a)
**   - For "a || b", tree is OR(left=b, right=a)
**   - For "a | b",  tree is PIPE(left=b, right=a)
**
** Therefore, we must traverse RIGHT first, then LEFT to maintain correct order.
*/

int	ast_traversal(t_ast *node, t_shell *shell)
{
	if (node == NULL || shell == NULL)
		return (0);
	if (node->type == NODE_AND)
	{
		ast_traversal(node->right, shell);
		if (shell->last_exit_status == 0)
			ast_traversal(node->left, shell);
		return (0);
	}
	else if (node->type == NODE_OR)
	{
		ast_traversal(node->right, shell);
		if (shell->last_exit_status != 0)
			ast_traversal(node->left, shell);
		return (0);
	}
	else if (node->type == NODE_PIPE)
		exec_pipe(node, shell);
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(node, shell);
	else
		exec_command(node, shell);
	return (0);
}
