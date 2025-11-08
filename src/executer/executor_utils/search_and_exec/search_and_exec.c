/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_and_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:44:05 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/08 20:44:53 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		xfree(envp[i]);
		i++;
	}
	xfree(envp);
}

static int	check_file_access(char *path, t_shell *shell)
{
	struct stat	st;

	if (access(path, F_OK) != 0)
	{
		write(2, "minishell: ", 6);
		write(2, path, strlen(path));
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
		write(2, path, strlen(path));
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

static void	exec_with_slash(t_shell *shell, char **cmd_args, char **env)
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

static void	set_last_arg(t_shell *shell, char **cmd_args)
{
	size_t	i;
	char	*last_arg;

	i = 0;
	while (cmd_args[i])
		i++;
	if (i > 0)
		last_arg = cmd_args[i - 1];
	else
		last_arg = cmd_args[0];
	set_variable(shell, "_", last_arg, 1);
}

void	search_and_exec(t_shell *shell, char **cmd_args)
{
	int		has_slash;
	char	**env;

	if (!cmd_args || !cmd_args[0])
		return ;
	set_last_arg(shell, cmd_args);
	has_slash = (ft_strchr(cmd_args[0], '/') != NULL);
	if (is_builtin(cmd_args[0]))
	{
		exec_builtin(shell, cmd_args);
		return ;
	}
	if (has_slash)
	{
		env = generate_envp(shell->env_list);
		if (!env)
		{
			shell->last_exit_status = 1;
			return ;
		}
		exec_with_slash(shell, cmd_args, env);
		return ;
	}
	search_in_path_and_exec(shell, cmd_args);
}
