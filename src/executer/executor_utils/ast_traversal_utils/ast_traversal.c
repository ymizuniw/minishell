#include "../../../../includes/minishell.h"

static void	handle_logical_and(t_ast *node, t_shell *shell)
{
	ast_traversal(node->right, shell);
	if (shell->last_exit_status == 0)
		ast_traversal(node->left, shell);
}

static void	handle_logical_or(t_ast *node, t_shell *shell)
{
	ast_traversal(node->right, shell);
	if (shell->last_exit_status != 0)
		ast_traversal(node->left, shell);
}

int	ast_traversal(t_ast *node, t_shell *shell)
{
	if (node == NULL || shell == NULL)
		return (0);
	if (node->type == NODE_AND)
		handle_logical_and(node, shell);
	else if (node->type == NODE_OR)
		handle_logical_or(node, shell);
	else if (node->type == NODE_PIPE)
		exec_pipe(node, shell);
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(node, shell);
	else
		exec_command(node, shell);
	return (0);
}
