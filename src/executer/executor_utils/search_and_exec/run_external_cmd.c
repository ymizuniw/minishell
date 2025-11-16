/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:09 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/16 22:28:50 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static void	exec_in_pipe_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	cleanup_before_execve(shell);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	exit(127);
}

static void	exec_in_fork_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	set_sig_dfl();
	cleanup_before_execve(shell);
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	exit(127);
}

		void set_sig_ign()
		{
			struct sigaction sact;
			sigemptyset(&sact.sa_mask);
			sact.sa_handler = SIG_IGN;
			sact.sa_flags = 0;
			sigaction(SIGTERM, &sact, NULL);
			sigaction(SIGQUIT, &sact, NULL);
			sigaction(SIGINT, &sact, NULL);
		}

void	search_in_path_and_exec(t_shell *shell, char **cmd_args)
{
	pid_t	pid;
	char	*cmd_path;
	char	**envp;

	if (!cmd_args || !cmd_args[0])
		return ;
	cmd_path = find_command_path(shell, cmd_args[0]);
	if (!cmd_path || !cmd_path[0])
		return ;
	envp = generate_envp(shell->env_list);
	if (!envp)
		return (xfree(cmd_path), (void)0);
	if (shell->in_pipe_child)
		return (exec_in_pipe_child(cmd_path, cmd_args, envp, shell));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), xfree(cmd_path), free_envp(envp),
			(void)(shell->last_exit_status = 1));
	if (pid>0)
	{
		set_sig_ign();
	}
	if (pid == 0)
		exec_in_fork_child(cmd_path, cmd_args, envp, shell);
	xfree(cmd_path);
	free_envp(envp);
	handle_child(&shell->last_exit_status, pid);
	signal_initializer(shell->interactive);
}

