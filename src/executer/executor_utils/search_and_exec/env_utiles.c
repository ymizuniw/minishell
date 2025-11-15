/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utiles.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:40 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:37:41 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*var;

	var = find_env(env_list, key);
	if (var)
		return (var->value);
	return (NULL);
}

void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		xfree(envp[i]);
		i++;
	}
	xfree(envp);
}
