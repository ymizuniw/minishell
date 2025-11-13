
#include "../../../includes/minishell.h"

static void	sort_env_array(t_env **arr, size_t count)
{
	size_t	i;
	size_t	j;
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

static size_t	count_exported_vars(t_env *env_list)
{
	t_env	*current;
	size_t	count;

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
//関数名かぶり

static t_env	**env_list_to_array(t_env *env_list, size_t count)
{
	t_env	**array;
	t_env	*current;
	size_t	i;

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

void	print_exported_vars(t_env *env_list, int fd)
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
