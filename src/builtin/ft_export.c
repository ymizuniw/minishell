#include "../../includes/minishell.h"

int	is_valid_varname(const char *var)
{
	int	i;

	i = 0;
	if (!var || !(isalpha(var[0]) || var[0] == '_'))
		return (0);
	while (var[i])
	{
		if (!(isalnum(var[i]) || var[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*value;
	t_env	*existing;

	key = extract_key(arg);
	if (!key)
		return (0);
	if (!is_valid_varname(key))
	{
		write(2, "export: `", 9);
		write(2, arg, strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		xfree(key);
		return (1);
	}
	value = extract_value(arg);
	if (value)
	{
		set_variable(shell, key, value, 1);
		xfree(value);
	}
	else
	{
		existing = find_env(shell->env_list, key);
		if (existing)
			existing->exported = 1;
		else
			set_variable(shell, key, "", 1);
	}
	xfree(key);
	return (0);
}

static void	sort_env_array(t_env **arr, int count)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	count_exported_vars(t_env *env_list)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env_list;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	return (count);
}

static t_env	**env_list_to_array(t_env *env_list, int count)
{
	t_env	**array;
	t_env	*current;
	int		i;

	array = xmalloc(sizeof(t_env *) * count);
	if (!array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		if (current->exported)
		{
			array[i] = current;
			i++;
		}
		current = current->next;
	}
	return (array);
}

static void	print_sorted_env(t_env **sorted, int count, int fd)
{
	int	i;

	i = 0;
	while (i < count)
	{
		ft_putstr_fd("declare -x ", fd);
		ft_putstr_fd(sorted[i]->key, fd);
		ft_putchar_fd('=', fd);
		ft_putchar_fd('"', fd);
		ft_putstr_fd(sorted[i]->value, fd);
		ft_putchar_fd('"', fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
}

static void	print_exported_vars(t_env *env_list, int fd)
{
	t_env	**sorted;
	int		count;

	count = count_exported_vars(env_list);
	if (count == 0)
		return ;
	sorted = env_list_to_array(env_list, count);
	if (!sorted)
		return ;
	sort_env_array(sorted, count);
	print_sorted_env(sorted, count, fd);
	xfree(sorted);
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
