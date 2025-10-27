#include "../../includes/minishell.h"

static bool	is_n_option(const char *arg)
{
	int	i;

	if (arg == NULL)
		return (false);
	if (arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

void	ft_echo(char **cmd)
{
	bool	newline;
	int		i;

	i = 1;
	newline = true;
	while (cmd[i] && is_n_option(cmd[i]))
	{
		newline = false;
		i++;
	}
	// chenge ft_printf
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
}
