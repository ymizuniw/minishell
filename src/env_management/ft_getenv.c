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

	current = env_list;
	while (current)
	{
		if (strncmp(current->key, key, strlen(key) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

char	*extract_key(const char *cmd)
{
	int		len;
	char	*key;

	len = 0;
	while (cmd[len] && cmd[len] != '=')
		len++;
	key = xmalloc(len + 1);
	if (!key)
		return (NULL);
	strncpy(key, cmd, len);
	key[len] = '\0';
	return (key);
}

char	*extract_value(const char *str)
{
	char	*equal_sign;

	equal_sign = strchr(str, '=');
	if (!equal_sign)
		return (NULL);
	return (strdup(equal_sign + 1));
}

void	set_variable(t_shell *shell, char *key, char *value, int exported)
{
	t_env	*current;
	t_env	*new;

	current = shell->env_list;
	while (current)
	{
		if (strncmp(current->key, key, strlen(key) + 1) == 0)
		{
			xfree(current->value);
			current->value = strdup(value);
			if (exported)
				current->exported = 1;
			return ;
		}
		current = current->next;
	}
	new = xmalloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = strdup(key);
	new->value = strdup(value);
	new->exported = exported;
	new->next = shell->env_list;
	shell->env_list = new;
}
