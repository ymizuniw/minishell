#include "../../../../includes/minishell.h"

// fork() and call ast_traversal for subtree. is execute flag is false,
// call ast_traversal with execute flag is false.
int	exec_subshell(t_ast *node, t_shell *shell, bool execute)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (node && node->subtree)
			return (ast_traversal(node->subtree, shell, execute));
		else
			return (0);
	}
	handle_child(&status, pid);
	return (0);
}
