/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 05:11:32 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*get_target_dir(char **cmd, t_shell *shell)
{
	t_env	*home_var;

	if (cmd[1])
		return (cmd[1]);
	home_var = find_env(shell->env_list, "HOME");
	if (!home_var)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
		return (NULL);
	}
	return (home_var->value);
}

static void	update_pwd(t_shell *shell)
{
	char	*new_pwd;

	set_variable(shell, "OLDPWD", shell->pwd, 1);
	new_pwd = get_pwd();
	if (new_pwd)
	{
		set_variable(shell, "PWD", new_pwd, 1);
		xfree(shell->pwd);
		shell->pwd = new_pwd;
	}
}

static int	cd_change_dir(char *target, t_shell *shell)
{
	char	*expanded;
	bool	allocated;

	allocated = false;
	if (target[0] == '\0')
		target = ".";
	expanded = expand_home(target, shell, &allocated);
	if (!expanded)
		return (1);
	if (chdir(expanded) == -1)
	{
		if (allocated)
			xfree(expanded);
		perror("cd");
		return (1);
	}
	if (allocated)
		xfree(expanded);
	return (0);
}

int	ft_cd(char **cmd, t_shell *shell)
{
	char	*target_dir;

	if (count_token(cmd) > 2)
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		return (1);
	}
	target_dir = get_target_dir(cmd, shell);
	if (!target_dir)
		return (1);
	if (cd_change_dir(target_dir, shell) != 0)
		return (1);
	update_pwd(shell);
	return (0);
}
