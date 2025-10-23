#include "../../../../includes/minishell.h"

int	exec_pipe(t_ast *node, char **env, bool execute, int last_exit_status)
{
	int		right_status;
	int		left_status;
	int		pip[2];
	int		ret;
	pid_t	left_pid;
	pid_t	right_pid;

	if (execute==false)
	{
		left_status = ast_traversal(node->left, env, execute, last_exit_status);
		right_status = ast_traversal(node->right, env, execute, last_exit_status);
		return (last_exit_status);
	}
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
		exit(ast_traversal(node->left, env, execute));
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
		exit(ast_traversal(node->right, env,execute));
	}
	close(pip[0]);
	close(pip[1]);
	handle_child(&left_status, left_pid);
	handle_child(&right_status, right_pid);
	return (right_status);
}
