/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_in_path_and_exec.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:57:09 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:58:07 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	search_in_path_and_exec(t_shell *shell, char **cmd_args)
{
	char	*cmd_path;
	char	**envp;

	if (!validate_args(cmd_args))
		return ;
	cmd_path = resolve_path(shell, cmd_args);
	if (!cmd_path)
		return ;
	envp = prepare_envp(shell, cmd_path);
	if (!envp)
		return ;
	if (shell->in_pipe_child)
	{
		run_in_pipe_child(cmd_path, cmd_args, envp, shell);
		return ;
	}
	if (spawn_child(shell, cmd_path, cmd_args, envp) < 0)
		return ;
	signal_initializer(shell->interactive);
}
