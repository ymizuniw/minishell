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

void	ft_exit(char **cmd)
{
	int	exit_code;

	exit_code = 0;
	printf("exit\n");
	if (cmd[1])
	{
		// cmd[1] が数値かどうか確認
		if (!is_numeric(cmd[1]))
		{
			print_err_num(cmd); // "exit: <arg>: numeric argument required"
			exit(2);            // 数字でない場合は 2 で終了
		}
		// cmd[2] があれば、複数引数と見なしてエラー
		if (cmd[2])
		{
			print_err_args(); // "exit: too many arguments"
			exit(2);          // 複数引数の場合は 2 で終了
		}
		// 数字が一つだけ来た場合、そのまま返す（256で割った余り）
		exit_code = atoi(cmd[1]);
		exit(exit_code % 256);
	}
	exit(0);
}
