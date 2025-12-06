/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_slash.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:43 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:19:47 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	process_access_err(char const *path, int *last_exit_status,
		void (*print_err)(const char *), int status)
{
	print_err(path);
	*last_exit_status = status;
	return (0);
}

void	print_no_such_file_or_dir_err(char const *path)
{
	write(2, "minishell: ", 6);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
}

void	print_permission_denied_err(const char *path)
{
	write(2, "minishell: ", 6);
	write(2, path, ft_strlen(path));
	write(2, ": Permission denied\n", 20);
}

void	print_is_dir_err(const char *path)
{
	perror(path);
}

void	print_stat_err(const char *path)
{
	(void)path;
	perror("stat: ");
}

static int	check_file_access(char *path, t_shell *shell)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
	{
		return (process_access_err(path, &shell->last_exit_status,
				print_no_such_file_or_dir_err, 127));
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		return (process_access_err(path, &shell->last_exit_status,
				print_is_dir_err, 126));
	}
	if (stat(path, &st) != 0)
	{
		return (process_access_err(path, &shell->last_exit_status,
				print_stat_err, 126));
	}
	if (access(path, X_OK) != 0)
	{
		return (process_access_err(path, &shell->last_exit_status,
				print_permission_denied_err, 126));
	}
	return (1);
}

static void	exec_in_child(char **cmd_args, char **env, t_shell *shell)
{
	set_sig_dfl();
	cleanup_before_execve(shell);
	execve(cmd_args[0], cmd_args, env);
	perror("execve");
	free_envp(env);
	exit(127);
}

static void	exec_direct(char **cmd_args, char **env, t_shell *shell)
{
	cleanup_before_execve(shell);
	execve(cmd_args[0], cmd_args, env);
	perror("execve");
	free_envp(env);
	exit(127);
}

void	exec_with_slash(t_shell *shell, char **cmd_args, char **env)
{
	pid_t	pid;

	if (!check_file_access(cmd_args[0], shell))
	{
		free_envp(env);
		return ;
	}
	if (shell->in_pipe_child)
		exec_direct(cmd_args, env, shell);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->last_exit_status = 1;
		free_envp(env);
		signal_initializer(shell->interactive);
		return ;
	}
	if (pid == 0)
		exec_in_child(cmd_args, env, shell);
	handle_child(&shell->last_exit_status, pid);
	signal_initializer(shell->interactive);
	free_envp(env);
}
