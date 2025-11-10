#include "../../includes/minishell.h"

int	ft_env(t_shell *shell, char **cmd)
{
	t_env	*current;
	size_t	len;

	len = 0;
	if (cmd[1] != NULL)
	{
		if (cmd[1])
			len = strlen(cmd[1]);
		write(2, "env: '", 6);
		write(2, cmd[1], len);
		write(2, "': No such file or directory\n", 30);
		return (127);
	}
	current = shell->env_list;
	while (current)
	{
		if (current->exported)
		{
			ft_putstr_fd(current->key, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(current->value, 1);
			ft_putchar_fd('\n', 1);
		}
		current = current->next;
	}
	return (0);
}
