#include "../../includes/minishell.h"

t_result	executor(t_ast *ast, t_shell *shell)
{
	t_result	res;

	ft_memset(&res, 0, sizeof(t_result));
	if (!shell)
		return (res);
	ast_traversal(ast, shell);
	res.root = ast;
	res.exit_code = shell->last_exit_status;
	return (res);
}
