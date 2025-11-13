#include "../../../includes/minishell.h"

int	is_valid_varname(const char *var)
{
	size_t	i;

	i = 0;
	if (!var || !(ft_isalpha(var[0]) || var[0] == '_'))
		return (0);
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_export_no_value(t_shell *shell, char *key)
{
	t_env	*existing;

	existing = find_env(shell->env_list, key);
	if (existing)
		existing->exported = 1;
	else
		set_variable(shell, key, "", 1);
}

static void	output_messeges(char *arg)
{
	size_t	len;

	len = 0;
	if (arg)
		len = ft_strlen(arg);
	write(2, "export: `", 9);
	write(2, arg, len);
	write(2, "': not a valid identifier\n", 26);
}

static int	handle_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;

	key = extract_key(arg);
	if (!key)
		return (0);
	if (!is_valid_varname(key))
	{
		output_messeges(arg);
		return (xfree(key), 1);
	}
	value = extract_value(arg);
	if (value)
	{
		set_variable(shell, key, value, 1);
		xfree(value);
	}
	else
		handle_export_no_value(shell, key);
	xfree(key);
	return (0);
}

int	ft_export(t_shell *shell, char **cmd, int fd)
{
	int	i;
	int	ret;

	ret = 0;
	if (!cmd[1])
	{
		print_exported_vars(shell->env_list, fd);
		return (0);
	}
	i = 1;
	while (cmd[i])
	{
		if (handle_export_arg(shell, cmd[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
