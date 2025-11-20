/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:06 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/20 19:05:48 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char	*create_env_string(const char *key, const char *value)
{
	char	*str;
	size_t	key_len;
	size_t	value_len;
	size_t	len;

	key_len = 0;
	value_len = 0;
	len = 0;
	if (key)
		key_len = ft_strlen(key);
	if (value)
		value_len = ft_strlen(value);
	len = key_len + value_len + 2;
	str = xcalloc(len);
	if (!str)
		return (NULL);
	ft_memcpy(str, key, key_len);
	ft_memcpy(str + key_len, "=", 1);
	ft_memcpy(str + key_len + 1, value, value_len);
	return (str);
}

static int	add_env_to_array(char **envp, t_env *env_list)
{
	t_env	*current;
	size_t	i;

	current = env_list;
	i = 0;
	while (current)
	{
		envp[i] = create_env_string(current->key, current->value);
		if (!envp[i])
			return (free_envp(envp), 0);
		i++;
		current = current->next;
	}
	envp[i] = NULL;
	return (1);
}

static size_t	count_env_list_size(t_env *env_list)
{
	size_t	size;

	size = 0;
	while (env_list)
	{
		size++;
		env_list = env_list->next;
	}
	return (size);
}

char	**generate_envp(t_env *env_list)
{
	char	**envp;
	size_t	count;

	count = count_env_list_size(env_list);
	envp = xmalloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (!add_env_to_array(envp, env_list))
		return (NULL);
	return (envp);
}
