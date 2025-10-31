#include "../../includes/minishell.h"

int	ft_cd(char **cmd, t_shell *shell)
{
	char	*target_dir;
	char	*new_pwd;
	t_env	*home_var;

	if (count_token(cmd) > 2)
	{
		write(STDOUT_FILENO, "cd: too many arguments\n", 23);
		return (1);
	}
	target_dir = cmd[1];
	if (!target_dir)
	{
		home_var = find_env(shell->env_list, "HOME");
		if (!home_var)
		{
			write(STDOUT_FILENO, "cd: HOME not set\n", 17);
			return (1);
		}
		target_dir = home_var->value;
	}
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return (1);
	}
	set_variable(shell, "OLDPWD", shell->pwd, 1);
	new_pwd = get_pwd();
	if (new_pwd)
	{
		set_variable(shell, "PWD", new_pwd, 1);
		xfree(shell->pwd);
		shell->pwd = new_pwd;
	}
	return (0);
}
