/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv_utiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:01 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:37:02 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_token(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

t_env	*find_env(t_env *env_list, const char *key)
{
	t_env	*current;
	size_t	key_len;

	key_len = 0;
	if (key)
		key_len = ft_strlen(key);
	current = env_list;
	while (current)
	{
		if (ft_strncmp(current->key, key, key_len + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*extract_key(const char *cmd)
{
	int	len;

	len = 0;
	while (cmd[len] && cmd[len] != '=')
		len++;
	return (ft_substr(cmd, 0, len));
}

char	*extract_value(const char *str)
{
	char	*equal_sign;

	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
		return (NULL);
	return (ft_strdup(equal_sign + 1));
}
