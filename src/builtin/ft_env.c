#include "../../includes/minishell.h"

void	ft_env(t_shell *shell, char **cmd)
{
	t_env	*current;

	if (cmd[1] != NULL)
	{
		write(2, "env: '", 6);
		write(2, cmd[1], strlen(cmd[1]));
		write(2, "': No such file or directory\n", 30);
		exit(127);
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
}
