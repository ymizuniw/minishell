#include "../../includes/minishell.h"

void	exec_builtin(t_shell *shell, char **cmd)
{
	int fd = 1; // 暫定
	if (!cmd || !shell)
		exit(1);
	if (strncmp(cmd[0], "echo", 5) == 0)
		ft_echo(cmd);
	else if (strncmp(cmd[0], "cd", 3) == 0)
		ft_cd(cmd, shell, fd);
	else if (strncmp(cmd[0], "pwd", 4) == 0)
		ft_pwd(shell, fd);
	else if (strncmp(cmd[0], "export", 7) == 0)
		ft_export(shell, cmd, fd);
	else if (strncmp(cmd[0], "unset", 6) == 0)
		ft_unset(shell, cmd);
	else if (strncmp(cmd[0], "env", 4) == 0)
		ft_env(shell, cmd);
	else if (strncmp(cmd[0], "exit", 5) == 0)
		ft_exit(cmd);
}
