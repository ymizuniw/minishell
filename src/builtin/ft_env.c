#include "../../includes/minishell.h"

int	ft_env(t_shell *shell, char **cmd)
{
	t_env	*current;
	size_t	len;

	len = 0;
	if (cmd[1] != NULL)
	{
		if (cmd[1])
			len = ft_strlen(cmd[1]);
		write(2, "env: '", 6);
		write(2, cmd[1], len);
		write(2, "': No such file or directory\n", 30);
		return (127);
	}
	current = shell->env_list;
	while (current)
	{
		if (current->exported)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
