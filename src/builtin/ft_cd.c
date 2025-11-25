/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/26 02:22:28 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	ft_cd(char **cmd, t_shell *shell)
{
	char	*target_dir;
	bool 	allocated=false;
	char *joined=NULL;
	
	if (count_token(cmd) > 2)
	{
		write(STDERR_FILENO, "cd: too many arguments\n", 23);
		return (1);
	}
	target_dir = get_target_dir(cmd, shell);
	if (!target_dir)
		return (1);
	if (target_dir[0]=='\0')
		target_dir=".";
	else if (ft_strcmp(target_dir, "~")==0)
	{
		t_env *home = find_env(shell->env_list, "HOME");
		if (!home)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
			return (1);
		}
		target_dir = home->value;
	}
	else if (target_dir[0]=='~' && target_dir[1]=='/')
	{
		t_env *home = find_env(shell->env_list, "HOME");
		if (!home)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
			return (1);
		}
		joined = ft_strjoin(home->value, target_dir+1);
		if (joined==NULL)
			return (1);
		allocated=true;
		target_dir = joined;
	}
	if (chdir(target_dir) == -1)
	{
		if (allocated)
			xfree(joined);
		perror("cd: ");
		return (1);
	}
	update_pwd(shell);
	if (allocated)
		xfree(joined);
	return (0);
}
