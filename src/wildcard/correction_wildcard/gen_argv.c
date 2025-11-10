
#include "minishell.h" 

static char	**handle_normal_arg(char **argv, size_t *capacity, size_t *argv_idx, char *expanded)
{
	char	**new_argv;

	if (*argv_idx >= *capacity)
	{
		*capacity *= 2;
		new_argv = realloc(argv, sizeof(char *) * (*capacity + 1));
		if (!new_argv)
		{
			free(expanded);
			return (NULL);
		}
		argv = new_argv;
	}
	argv[*argv_idx] = expanded;
	(*argv_idx)++;
	return (argv);
}

static char	**fill_argv_array(char **argv, t_argv *argv_list, size_t *capacity, t_shell *shell)
{
	t_argv	*cur_argv;
	size_t	argv_idx;
	char	*expanded;

	cur_argv = argv_list;
	argv_idx = 0;
	while (cur_argv != NULL)
	{
		if (cur_argv->to_expand == true)
			expanded = expand_word(cur_argv->word, shell);
		else
			expanded = strdup(cur_argv->word);
		if (strchr(expanded, '*') && cur_argv->to_wildcard_expand)
		{
			argv = handle_wildcard_expansion(argv, capacity, &argv_idx, expanded, shell);
			if (!argv)
				return (NULL);
		}
		else
		{
			argv = handle_normal_arg(argv, capacity, &argv_idx, expanded);
			if (!argv)
				return (NULL);
		}
		cur_argv = cur_argv->next;
	}
	argv[argv_idx] = NULL;
	return (argv);
}

static size_t	count_argv_list(t_argv *argv_list)
{
	t_argv	*cur;
	size_t	count;

	count = 0;
	cur = argv_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

char	**gen_argv(t_argv *argv_list, t_shell *shell)
{
	size_t	list_len;
	char	**argv;

	if (argv_list == NULL)
		return (NULL);
	list_len = count_argv_list(argv_list);
	argv = xmalloc(sizeof(char *) * (list_len + 1));
	if (!argv)
		return (NULL);
	argv = fill_argv_array(argv, argv_list, &list_len, shell);
	return (argv);
}
