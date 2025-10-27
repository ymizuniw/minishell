#include "../../includes/minishell.h"

void	init_env_from_envp(t_shell *shell, char **envp)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		value = extract_value(envp[i]);
		set_variable(shell, key, value, 1);
		xfree(key);
		xfree(value);
		i++;
	}
}

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		xfree(current->key);
		xfree(current->value);
		xfree(current);
		current = next;
	}
}
