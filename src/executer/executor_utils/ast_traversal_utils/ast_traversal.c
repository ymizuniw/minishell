#include "../../../../includes/minishell.h"

int	ast_traversal(t_ast *node, char **env)
{
	int	last_exit_status;

	if (node == NULL)
		return (0);
	last_exit_status = 0;
	if (node->type == NODE_AND)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status == 0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (node->type == NODE_OR)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status != 0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (node->type == NODE_PIPE)
		last_exit_status = exec_pipe(node, env);
	else if (node->type == NODE_SUBSHELL)
		last_exit_status = exec_subshell(node, env);
	else
		last_exit_status = exec_command(node, env);
	return (last_exit_status);
}
