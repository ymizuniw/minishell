/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_and_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:44:05 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/21 01:16:14 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
