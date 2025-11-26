/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:47 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/27 02:58:38 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (!ft_isdigit(str[i]))
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

	write(2, prefix, ft_strlen(prefix));
	if (cmd[1])
	{
		len = ft_strlen(cmd[1]);
		write(2, cmd[1], len);
	}
	write(2, suffix, ft_strlen(suffix));
}

void	print_err_args(void)
{
	const char	*msg = "exit: too many arguments\n";

	write(2, msg, ft_strlen(msg));
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
	free_double_array(cmd);
	free_shell(shell);
	disable_raw_mode(&shell->orig_term);
	exit(code);
}

__attribute__((noreturn))void	ft_exit(char **cmd, int last_exit_status, t_shell *shell)
{
	long long	exit_code;
	int			overflow=0;

	if (shell->interactive)
		printf("exit\n");
	if (cmd && cmd[1] && cmd[1][0] != '\0')
	{
		if (!is_numeric(cmd[1]))
			exit_with_error(shell, 2, true, cmd);
		if (cmd[2])
			exit_with_error(shell, 1, false, cmd);
		exit_code = ft_atoll(cmd[1], &overflow);
		if (overflow)
			exit_with_error(shell, 2, true, cmd);
		free_double_array(cmd);
		free_shell(shell);
		exit_code %= 256;
		if (exit_code<0)
			exit_code += 256;
		exit(exit_code);
	}
	free_double_array(cmd);
	disable_raw_mode(&shell->orig_term);
	free_shell(shell);
	exit(last_exit_status);
}
