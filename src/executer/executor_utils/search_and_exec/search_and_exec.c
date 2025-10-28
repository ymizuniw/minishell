/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_and_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:44:05 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/28 22:59:44 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	search_and_exec(t_shell *shell, char **cmd_args)
{
	int		has_slash;
	char	**env;

	if (!cmd_args || !cmd_args[0])
		return ;
	has_slash = (ft_strchr(cmd_args[0], '/') != NULL);
	env = generate_envp(shell->env_list);
	if (!env)
		exit(1);
	if (is_builtin(cmd_args[0]))
		run_builtin(shell, cmd_args);
	else if (has_slash)
	{
		if (access(cmd_args[0], F_OK) != 0)
		{
			write(2, "bash: ", 6);
			write(2, cmd_args[0], ft_strlen(cmd_args[0]));
			write(2, ": No such file or directory\n", 28);
			shell->last_exit_status = 127;
		}
		else if (access(cmd_args[0], X_OK) != 0)
		{
			write(2, "bash: ", 6);
			write(2, cmd_args[0], ft_strlen(cmd_args[0]));
			write(2, ": Permission denied\n", 20);
			shell->last_exit_status = 126;
		}
		else
		{
			execve(cmd_args[0], cmd_args, env);
			// execve が戻ったら何らかの理由で失敗
			write(2, "bash: ", 6);
			write(2, cmd_args[0], ft_strlen(cmd_args[0]));
			write(2, ": execution failed\n", 19);
			shell->last_exit_status = 127;
		}
	}
	else
		search_in_path_and_exec(shell, cmd_args);
}
