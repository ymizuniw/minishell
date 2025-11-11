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
	current = env_list;
	while (current)
	{
		if (key)
			key_len = strlen(key);
		if (strncmp(current->key, key, key_len + 1) == 0)
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

static int	update_existing_var(t_env *current, char *value, int exported)
{
	char	*dup;

	dup = strdup(value);
	if (!dup)
		return (0);
	xfree(current->value);
	current->value = dup;
	if (exported)
		current->exported = 1;
	return (1);
}

static t_env	*create_new_env(char *key, char *value, int exported)
{
	t_env	*new;

	new = xmalloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = strdup(key);
	if (!new->key)
		return (xfree(new), NULL);
	new->value = strdup(value);
	if (!new->value)
		return (xfree(new->key), xfree(new), NULL);
	new->exported = exported;
	new->next = NULL;
	return (new);
}

void	set_variable(t_shell *shell, char *key, char *value, int exported)
{
	t_env	*current;
	t_env	*new;
	size_t	key_len;

	key_len = 0;
	if (!shell || !key || !value)
		return ;
	current = shell->env_list;
	while (current)
	{
		if (key)
			key_len = strlen(key);
		if (strncmp(current->key, key, key_len + 1) == 0)
		{
			update_existing_var(current, value, exported);
			return ;
		}
		current = current->next;
	}
	new = create_new_env(key, value, exported);
	if (!new)
		return ;
	new->next = shell->env_list;
	shell->env_list = new;
}
