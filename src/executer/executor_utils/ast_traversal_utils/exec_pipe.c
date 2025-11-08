#include "../../../../includes/minishell.h"

static void	exec_pipe_right_child(int *pip, t_ast *node, t_shell *shell)
{
	set_sig_dfl();
	shell->in_pipe_child = true;
	close(pip[0]);
	if (dup2(pip[1], STDOUT_FILENO) < 0)
		ft_exit(NULL, 1, shell);
	close(pip[1]);
	ast_traversal(node->right, shell);
	exit(shell->last_exit_status);
}

static void	exec_pipe_left_child(int *pip, t_ast *node, t_shell *shell)
{
	set_sig_dfl();
	shell->in_pipe_child = true;
	close(pip[1]);
	if (dup2(pip[0], STDIN_FILENO) < 0)
		ft_exit(NULL, shell->last_exit_status, shell);
	close(pip[0]);
	ast_traversal(node->left, shell);
	exit(shell->last_exit_status);
}

static int	fork_pipe_right(int *pip, t_ast *node, t_shell *shell)
{
	pid_t	right_pid;

	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return (-1);
	}
	if (right_pid == 0)
		exec_pipe_right_child(pip, node, shell);
	return (right_pid);
}

static int	fork_pipe_left(int *pip, t_ast *node, t_shell *shell, pid_t rpid)
{
	pid_t	left_pid;

	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		kill(rpid, SIGTERM);
		waitpid(rpid, NULL, 0);
		return (-1);
	}
	if (left_pid == 0)
		exec_pipe_left_child(pip, node, shell);
	return (left_pid);
}

int	exec_pipe(t_ast *node, t_shell *shell)
{
	int		pip[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node || !shell || pipe(pip) < 0)
		return (perror("pipe"), 1);
	right_pid = fork_pipe_right(pip, node, shell);
	if (right_pid < 0)
		return (-1);
	left_pid = fork_pipe_left(pip, node, shell, right_pid);
	if (left_pid < 0)
		return (-1);
	close(pip[0]);
	close(pip[1]);
	handle_child(&shell->last_exit_status, right_pid);
	handle_child(&shell->last_exit_status, left_pid);
	return (0);
}
