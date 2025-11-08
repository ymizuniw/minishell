#include "../../includes/minishell.h"

static char	*get_target_dir(char **cmd, t_shell *shell)
{
	t_env	*home_var;

	if (cmd[1])
		return (cmd[1]);
	home_var = find_env(shell->env_list, "HOME");
	if (!home_var)
	{
		write(STDOUT_FILENO, "cd: HOME not set\n", 17);
		return (NULL);
	}
	return (home_var->value);
}

static void	update_pwd(t_shell *shell)
{
	char	*new_pwd;

	set_variable(shell, "OLDPWD", shell->pwd, 1);
	new_pwd = get_pwd();
	if (new_pwd)
	{
		set_variable(shell, "PWD", new_pwd, 1);
		xfree(shell->pwd);
		shell->pwd = new_pwd;
	}
}

int	ft_cd(char **cmd, t_shell *shell)
{
	char	*target_dir;

	if (count_token(cmd) > 2)
	{
		write(STDOUT_FILENO, "cd: too many arguments\n", 23);
		return (1);
	}
	target_dir = get_target_dir(cmd, shell);
	if (!target_dir)
		return (1);
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return (1);
	}
	update_pwd(shell);
	return (0);
}
