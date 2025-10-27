#include "../../../../includes/minishell.h"

int	exec_pipe(t_ast *node, t_shell *shell, bool execute)
{
	int		pip[2];
	int		ret;
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node)
		return (0);
	if (!execute)
	{
		// Dry-run mode for tests: do not fork, just traverse
		ast_traversal(node->left, shell, execute);
		ast_traversal(node->right, shell, execute);
		return (0);
	}
	ret = pipe(pip);
	if (ret < 0)
	{
		perror("pipe");
		return (1);
	}
	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return (-1);
	}
	if (left_pid == 0)
	{
		close(pip[0]);
		dup2(pip[1], STDOUT_FILENO);
		close(pip[1]);
		ast_traversal(node->left, shell, execute);
		exit(shell->last_exit_status);
	}
	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
		return (-1);
	}
	if (right_pid == 0)
	{
		close(pip[1]);
		dup2(pip[0], STDIN_FILENO);
		close(pip[0]);
		ast_traversal(node->right, shell, execute);
		exit (shell->last_exit_status);
	}
	close(pip[0]);
	close(pip[1]);
	handle_child(&shell->last_exit_status, left_pid);
	handle_child(&shell->last_exit_status, right_pid);
	return (0);
}
