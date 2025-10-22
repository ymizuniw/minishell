#include "../../../../includes/minishell.h"

// fork() and call ast_traversal for subtree.
int	exec_subshell(t_ast *node, char **env)
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
			exit(ast_traversal(node->subtree, env));
		else
			exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (EXIT_FAILURE);
}
