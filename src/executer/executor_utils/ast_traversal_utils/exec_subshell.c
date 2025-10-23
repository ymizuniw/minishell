#include "../../../../includes/minishell.h"

// fork() and call ast_traversal for subtree.
int	exec_subshell(t_ast *node, char **env, bool execute, int last_exit_status)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (execute==false)
		return (ast_traversal(node->subtree, env, execute, last_exit_status));
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (node && node->subtree)
			return (ast_traversal(node->subtree, env, execute, last_exit_status));
		else
			return (0);
	}
	handle_child(&status, pid);
	return (status);
}
