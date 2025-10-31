#include "../../includes/minishell.h"

bool	is_numeric(const char *str)
{
	int	i;

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

	write(2, prefix, strlen(prefix));
	write(2, cmd[1], strlen(cmd[1]));
	write(2, suffix, strlen(suffix));
}

void	print_err_args(void)
{
	const char	*msg = "exit: too many arguments\n";

	write(2, msg, strlen(msg));
}

void	ft_exit(char **cmd, int last_exit_status)
{
	int	exit_code;

	printf("exit\n");
	if (cmd[1])
	{
		if (!is_numeric(cmd[1]))
		{
			print_err_num(cmd);
			exit(2);
		}
		if (cmd[2])
		{
			print_err_args();
			exit(2);
		}
		exit_code = atoi(cmd[1]);
		exit(exit_code % 256);
	}
	exit(last_exit_status);
}
