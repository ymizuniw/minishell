#include "../../../../includes/minishell.h"

int	exec_pipe(t_ast *node, char **env)
{
	int		right_status;
	int		left_status;
	int		pip[2];
	int		ret;
	pid_t	left_pid;
	pid_t	right_pid;

	right_status = 0;
	left_status = 0;
	ret = pipe(pip);
	if (ret < 0)
	{
		perror("pipe");
		return (1);
	}
	if (node->pipeline)
	{
		node->pipeline->in_fd = pip[0];
		node->pipeline->out_fd = pip[1];
	}
	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return (1);
	}
	if (left_pid == 0)
	{
		close(pip[0]);
		dup2(pip[1], STDOUT_FILENO);
		close(pip[1]);
		exit(ast_traversal(node->left, env));
	}
	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		kill(left_pid, SIGTERM);
		waitpid(left_pid, NULL, 0);
		return (1);
	}
	if (right_pid == 0)
	{
		close(pip[1]);
		dup2(pip[0], STDIN_FILENO);
		close(pip[0]);
		exit(ast_traversal(node->right, env));
	}
	close(pip[0]);
	close(pip[1]);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status))
		return (WEXITSTATUS(right_status));
	else
		return (EXIT_FAILURE);
}
