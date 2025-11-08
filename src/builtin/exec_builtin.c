#include "../../includes/minishell.h"

void	exec_builtin(t_shell *shell, char **cmd)
{
	int	fd;
	int	ret;

	fd = 1;
	ret = 0;
	if (!cmd || !shell)
		ft_exit(cmd, shell->last_exit_status, shell);
	if (strcmp(cmd[0], "echo") == 0)
		ret = ft_echo(cmd);
	else if (strcmp(cmd[0], "cd") == 0)
		ret = ft_cd(cmd, shell);
	else if (strcmp(cmd[0], "pwd") == 0)
		ret = ft_pwd(shell, fd);
	else if (strcmp(cmd[0], "export") == 0)
		ret = ft_export(shell, cmd, fd);
	else if (strcmp(cmd[0], "unset") == 0)
		ret = ft_unset(shell, cmd);
	else if (strcmp(cmd[0], "env") == 0)
		ret = ft_env(shell, cmd);
	else if (strcmp(cmd[0], "exit") == 0)
	{
		ft_exit(cmd, shell->last_exit_status, shell);
	}
	shell->last_exit_status = ret;
}
