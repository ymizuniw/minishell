/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_home.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 18:31:20 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/01 18:36:21 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static t_env	*find_home(t_shell *shell)
{
	t_env	*home;

	home = find_env(shell->env_list, "HOME");
	if (!home)
	{
		write(STDERR_FILENO, "cd: HOME not set\n", 17);
		return (NULL);
	}
	return (home);
}

char	*expand_home(char *target_dir, t_shell *shell, bool *allocated)
{
	t_env	*home;
	char	*joined;

	*allocated = false;
	joined = NULL;
	if (ft_strcmp(target_dir, "~") == 0)
	{
		home = find_home(shell);
		if (!home)
			return (NULL);
		return (home->value);
	}
	if (target_dir[0] == '~' && target_dir[1] == '/')
	{
		home = find_home(shell);
		if (!home)
			return (NULL);
		joined = ft_strjoin(home->value, target_dir + 1);
		if (!joined)
			return (NULL);
		*allocated = true;
		return (joined);
	}
	return (target_dir);
}
