/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:24 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/16 20:19:28 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_command(char **cmd, t_shell *shell, int fd)
{
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (ft_cd(cmd, shell));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (ft_pwd(shell, fd));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (ft_export(shell, cmd, fd));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (ft_unset(shell, cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (ft_env(shell, cmd));
	return (0);
}

void	exec_builtin(t_shell *shell, char **cmd)
{
	int	fd;
	int	ret;

	fd = 1;
	ret = 0;
	if (!cmd && shell)
		ft_exit(cmd, shell->last_exit_status, shell);
	else if (!shell)
	{
		write(2, "fatal shell variable is NULL\n", 29);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(cmd[0], "exit") == 0)
		ft_exit(cmd, shell->last_exit_status, shell);
	ret = execute_command(cmd, shell, fd);
	shell->last_exit_status = ret;
}
