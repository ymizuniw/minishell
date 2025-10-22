#include "../../includes/minishell.h"

t_result	*executor(t_ast *node, char **env)
{
	int			status;
	t_result	*res;

	status = 0;
	status = ast_traversal(node, env);
	res = alloc_result();
	if (!res)
		return (NULL);
	res->exit_code = status;
	res->root = node;
	return (res);
}
