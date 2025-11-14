#include "../../../../includes/minishell.h"

static int	check_file_access(char *path, t_shell *shell)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
	{
		write(2, "minishell: ", 6);
		write(2, path, ft_strlen(path));
		write(2, ": No such file or directory\n", 28);
		shell->last_exit_status = 127;
		return (0);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (perror(path), shell->last_exit_status = 126, 0);
	if (stat(path, &st) != 0)
		return (perror("stat: "), shell->last_exit_status = 126, 0);
	if (access(path, X_OK) != 0)
	{
		write(2, "minishell: ", 6);
		write(2, path, ft_strlen(path));
		write(2, ": Permission denied\n", 20);
		shell->last_exit_status = 126;
		return (0);
	}
	return (1);
}

static void	exec_in_child(char **cmd_args, char **env)
{
	set_sig_dfl();
	execve(cmd_args[0], cmd_args, env);
	perror("execve");
	free_envp(env);
	exit(127);
}

static void	exec_direct(char **cmd_args, char **env)
{
	execve(cmd_args[0], cmd_args, env);
	perror("execve");
	free_envp(env);
	exit(127);
}

void	exec_with_slash(t_shell *shell, char **cmd_args, char **env)
{
	pid_t	pid;

	if (!check_file_access(cmd_args[0], shell))
		return ;
	if (shell->in_pipe_child)
	{
		exec_direct(cmd_args, env);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->last_exit_status = 1;
		free_envp(env);
		return ;
	}
	if (pid == 0)
		exec_in_child(cmd_args, env);
	handle_child(&shell->last_exit_status, pid);
	free_envp(env);
}
