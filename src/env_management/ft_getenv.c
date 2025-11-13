#include "../../includes/minishell.h"

static int	update_existing_var(t_env *current, char *value, int exported)
{
	char	*dup;

	dup = ft_strdup(value);
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
	new->key = ft_strdup(key);
	if (!new->key)
		return (xfree(new), NULL);
	new->value = ft_strdup(value);
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
	if (key)
		key_len = ft_strlen(key);
	current = shell->env_list;
	while (current)
	{
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
