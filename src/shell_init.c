/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:42:48 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/16 22:10:35 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_shell_env(t_shell *shell, char **env)
{
	char	*pwd;

	init_env_from_envp(shell, env);
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		shell->pwd = pwd;
		set_variable(shell, "PWD", pwd, 1);
	}
	shell->last_exit_status = 0;
	set_variable(shell, "_", "/usr/bin/minishell", 1);
}

void	init_shell(t_shell *shell, char **env)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	if (isatty(STDIN_FILENO) == 1 && isatty(STDOUT_FILENO))
		shell->interactive = true;
	signal_initializer(shell->interactive);
	init_shell_env(shell, env);
}
