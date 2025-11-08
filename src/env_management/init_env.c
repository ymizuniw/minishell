#include "../../includes/minishell.h"

static int	process_env_entry(t_shell *shell, char *envp_entry)
{
	char	*key;
	char	*value;

	key = extract_key(envp_entry);
	if (key == NULL)
		return (0);
	value = extract_value(envp_entry);
	if (value == NULL)
	{
		value = strdup("");
		if (!value)
			return (xfree(key), 0);
	}
	set_variable(shell, key, value, 1);
	xfree(key);
	xfree(value);
	return (1);
}

void	init_env_from_envp(t_shell *shell, char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		process_env_entry(shell, envp[i]);
		i++;
	}
}
