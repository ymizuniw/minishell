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
