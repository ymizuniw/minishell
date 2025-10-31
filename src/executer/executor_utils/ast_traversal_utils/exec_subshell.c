#include "../../../../includes/minishell.h"

// fork() and call ast_traversal for subtree. is execute flag is false,
// call ast_traversal with execute flag is false.
int	exec_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		set_sig_dfl();
		if (node && node->subtree)
			ast_traversal(node->subtree, shell);
		_exit(shell->last_exit_status);
	}
	handle_child(&shell->last_exit_status, pid);
	return (0);
}
