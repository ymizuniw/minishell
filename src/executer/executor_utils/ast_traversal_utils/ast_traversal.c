#include "../../../../includes/minishell.h"

int	ast_traversal(t_ast *node, t_shell *shell, bool execute)
{
	if (node == NULL)
		return (0);
	if (node->type == NODE_AND)
	{
		ast_traversal(node->left, shell, execute);
		if (shell->last_exit_status == 0)
			ast_traversal(node->right, shell, execute);
		return (0);
	}
	else if (node->type == NODE_OR)
	{
		ast_traversal(node->left, shell, execute);
		if (shell->last_exit_status != 0)
			ast_traversal(node->right, shell, execute);
		return (0);
	}
	else if (node->type == NODE_PIPE)
		exec_pipe(node, shell, execute);
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(node, shell, execute);
	else
		exec_command(node, shell, execute);
	return (0);
}
