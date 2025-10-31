#include "../../includes/minishell.h"

static void	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !key)
		return ;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (strncmp(current->key, key, strlen(key) + 1) == 0)
		{
			if (!prev)
				*env_list = current->next;
			else
				prev->next = current->next;
			xfree(current->key);
			xfree(current->value);
			xfree(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_shell *shell, char **cmd)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (cmd[i])
	{
		if (!is_valid_varname(cmd[i]))
		{
			write(2, "unset: `", 8);
			write(2, cmd[i], strlen(cmd[i]));
			write(2, "': not a valid identifier\n", 26);
			ret = 1;
		}
		else
			remove_env_var(&shell->env_list, cmd[i]);
		i++;
	}
	return (ret);
}
