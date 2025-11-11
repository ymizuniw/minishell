#include "../../includes/minishell.h"

bool	is_numeric(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (false);
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

void	print_err_num(char **cmd)
{
	const char	*prefix = "exit: ";
	const char	*suffix = ": numeric argument required\n";
	size_t		len;

	len = 0;
	write(2, prefix, strlen(prefix));
	if (cmd[1])
	{
		len = strlen(cmd[1]);
		write(2, cmd[1], len);
	}
	write(2, suffix, strlen(suffix));
}

void	print_err_args(void)
{
	const char	*msg = "exit: too many arguments\n";

	write(2, msg, strlen(msg));
}

static void	exit_with_error(t_shell *shell, int code, bool print_num_err,
		char **cmd)
{
	if (shell->interactive)
	{
		if (print_num_err)
			print_err_num(cmd);
		else
			print_err_args();
	}
	free_shell(shell);
	exit(code);
}

void	ft_exit(char **cmd, int last_exit_status, t_shell *shell)
{
	int	exit_code;

	if (shell->interactive)
		printf("exit\n");
	if (cmd && cmd[1] && cmd[1][0] != '\0')
	{
		if (!is_numeric(cmd[1]))
			exit_with_error(shell, 2, true, cmd);
		if (cmd[2])
			exit_with_error(shell, 2, false, cmd);
		exit_code = atoi(cmd[1]);
		free_shell(shell);
		exit(exit_code % 256);
	}
	free_shell(shell);
	exit(last_exit_status);
}
