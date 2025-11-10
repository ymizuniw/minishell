#include "../../includes/minishell.h"

static void	free_env_node(t_env *node)
{
	xfree(node->key);
	xfree(node->value);
	xfree(node);
}

static void	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;
	size_t	len;

	len = 0;
	if (!env_list || !*env_list || !key)
		return ;
	if (key)
		len = strlen(key);
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (strncmp(current->key, key, len + 1) == 0)
		{
			if (!prev)
				*env_list = current->next;
			else
				prev->next = current->next;
			return (free_env_node(current));
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_shell *shell, char **cmd)
{
	size_t	i;
	size_t	len;
	int		ret;

	len = 0;
	i = 1;
	ret = 0;
	while (cmd[i])
	{
		if (!is_valid_varname(cmd[i]))
		{
			if (cmd[i])
				len = strlen(cmd[i]);
			write(2, "unset: `", 8);
			write(2, cmd[i], len);
			write(2, "': not a valid identifier\n", 26);
			ret = 1;
		}
		else
			remove_env_var(&shell->env_list, cmd[i]);
		i++;
	}
	return (ret);
}
