#include "../../includes/minishell.h"

t_result	*executor(t_ast *ast, t_shell *shell)
{
	t_result	*res;

	res = alloc_result();
	ast_traversal(ast, shell, true);
	res->root = ast;
	res->exit_code = shell->last_exit_status;
	return (res);
}
