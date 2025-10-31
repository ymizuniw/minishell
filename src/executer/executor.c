#include "../../includes/minishell.h"

t_result	*executor(t_ast *ast, t_shell *shell)
{
	t_result	*res;

	res = alloc_result();
	if (!res || !shell)
		return (NULL);
	// Execute the AST
	ast_traversal(ast, shell);
	res->root = ast;
	res->exit_code = shell->last_exit_status;
	return (res);
}
