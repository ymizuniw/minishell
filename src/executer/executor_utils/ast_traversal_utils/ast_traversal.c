#include "../../../../includes/minishell.h"

int	ast_traversal(t_ast *node, char **env, bool execute, int last_exit_status)
{
	if (node == NULL)
		return (0);
	if (node->type == NODE_AND)
	{
		last_exit_status = ast_traversal(node->left, env, execute, last_exit_status);
		if (last_exit_status == 0)
			execute = true;
		last_exit_status = ast_traversal(node->right, env, execute, last_exit_status);
	}
	else if (node->type == NODE_OR)
	{
		last_exit_status = ast_traversal(node->left, env, execute, last_exit_status);
		if (last_exit_status != 0)
			execute = true;
		last_exit_status = ast_traversal(node->right, env, execute, last_exit_status);
	}
	else if (node->type == NODE_PIPE)
		last_exit_status = exec_pipe(node, env, execute, last_exit_status);
	else if (node->type == NODE_SUBSHELL)
		last_exit_status = exec_subshell(node, env, execute, last_exit_status);
	else
		last_exit_status = exec_command(node, env, execute, last_exit_status);
	return (last_exit_status);
}
