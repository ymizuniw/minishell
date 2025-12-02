/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:16:03 by ymizuniw         ###   ########.fr       */
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

// t_env	*find_home(t_shell *shell)
// {
// 	t_env	*home;

// 	home = find_env(shell->env_list, "HOME");
// 	if (!home)
// 	{
// 		write(STDERR_FILENO, "cd: HOME not set\n", 17);
// 		return (NULL);
// 	}
// 	return (home);
// }

// char	*expand_home(char *target_dir, t_shell *shell, bool *allocated)
// {
// 	t_env	*home;
// 	char	*joined;

// 	*allocated = false;
// 	joined = NULL;
// 	if (ft_strcmp(target_dir, "~") == 0)
// 	{
// 		home = find_home(shell);
// 		if (!home)
// 			return (NULL);
// 		return (home->value);
// 	}
// 	if (target_dir[0] == '~' && target_dir[1] == '/')
// 	{
// 		home = find_home(shell);
// 		if (!home)
// 			return (NULL);
// 		joined = ft_strjoin(home->value, target_dir + 1);
// 		if (!joined)
// 			return (NULL);
// 		*allocated = true;
// 		return (joined);
// 	}
// 	return (target_dir);
// }

// int	ft_cd(char **cmd, t_shell *shell)
// {
// 	char	*target_dir;
// 	bool	allocated;
// 	char	*expanded;

// 	allocated = false;
// 	if (count_token(cmd) > 2)
// 	{
// 		write(STDERR_FILENO, "cd: too many arguments\n", 23);
// 		return (1);
// 	}
// 	// return (write(STDERR_FILENO, "cd: too many arguments\n", 23), 1);
// 	target_dir = get_target_dir(cmd, shell);
// 	if (!target_dir)
// 		return (1);
// 	if (target_dir[0] == '\0')
// 		target_dir = ".";
// 	expanded = expand_home(target_dir, shell, &allocated);
// 	if (!expanded)
// 		return (1);
// 	if (chdir(expanded) == -1)
// 	{
// 		if (allocated)
// 			xfree(expanded);
// 		peror("cd");
// 		return (1);
// 		// return (perror("cd"), 1);
// 	}
// 	update_pwd(shell);
// 	if (allocated)
// 		xfree(expanded);
// 	return (0);
// }

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
