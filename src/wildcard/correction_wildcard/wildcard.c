
#include "minishell.h"

void	free_double_array(char **arr)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	append_wildcard(t_wildcard *head, t_wildcard *new)
{
	if (!head || !new)
		return (0);
	while (head->next)
		head = head->next;
	head->next = new;
	return (1);
}

bool	match_prefix(const char *name, const char *prefix, size_t *start)
{
	size_t	prefix_len;

	if (prefix[0] == '\0')
		return (true);
	prefix_len = strlen(prefix);
	if (strncmp(name, prefix, prefix_len) != 0)
		return (false);
	*start = prefix_len;
	return (true);
}

bool	match_middle(const char *name, char **keys, size_t key_count,
		size_t *start)
{
	size_t	i;
	char	*found;

	i = 1;
	while (i + 1 < key_count)
	{
		found = strstr(name + *start, keys[i]);
		if (!found)
			return (false);
		*start = (found - name) + strlen(keys[i]);
		i++;
	}
	return (true);
}

bool	match_suffix(const char *name, const char *last)
{
	size_t	name_len;
	size_t	last_len;

	name_len = strlen(name);
	if (last[0] == '\0')
		return (true);
	last_len = strlen(last);
	if (name_len < last_len)
		return (false);
	return (strcmp(name + name_len - last_len, last) == 0);
}

bool	match_to_keys(char **keys, const char *name)
{
	size_t	key_count;
	size_t	start;

	key_count = 0;
	start = 0;
	while (keys[key_count])
		key_count++;
	if (key_count == 1)
		return (strcmp(keys[0], name) == 0);
	if (!match_prefix(name, keys[0], &start))
		return (false);
	if (!match_middle(name, keys, key_count, &start))
		return (false);
	return (match_suffix(name, keys[key_count - 1]));
}

int	mark_wc_to_expand(const char *pattern, t_wildcard *wc)
{
	char		**keys;
	t_wildcard	*cur;

	cur = wc;
	keys = ft_split(pattern, '*');
	if (!keys)
		return (0);
	while (cur)
	{
		cur->expand = match_to_keys(keys, cur->name);
		cur = cur->next;
	}
	free_double_array(keys);
	return (1);
}

void	free_wildcard_list(t_wildcard *head)
{
	t_wildcard	*next;

	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

t_wildcard	*expand_wildcard(const char *pattern, const char *path)
{
	DIR				*dirp;
	t_wildcard		*head;
	t_wildcard		*tail;
	struct dirent	*dent;
	t_wildcard		*new;

	dirp = opendir(path);
	if (!dirp)
		return (NULL);
	head = NULL;
	tail = NULL;
	while ((dent = readdir(dirp)) != NULL)
	{
		if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue ;
		new = xmalloc(sizeof(t_wildcard));
		memset(new, 0, sizeof(t_wildcard));
		strncpy(new->name, dent->d_name, sizeof(new->name) - 1);
		new->expand = false;
		new->next = NULL;
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	closedir(dirp);
	if (head)
		mark_wc_to_expand(pattern, head);
	return (head);
}

static void	fill_wildcard_results(char **argv, size_t *argv_idx, t_wildcard *wild)
{
	t_wildcard	*cur;

	cur = wild;
	while (cur)
	{
		if (cur->expand)
		{
			argv[*argv_idx] = strdup(cur->name);
			(*argv_idx)++;
		}
		cur = cur->next;
	}
}

static size_t	count_wildcard_matches(t_wildcard *wild)
{
	t_wildcard	*cur;
	size_t		count;

	count = 0;
	cur = wild;
	while (cur)
	{
		if (cur->expand)
			count++;
		cur = cur->next;
	}
	return (count);
}

char	**handle_wildcard_expansion(char **argv, size_t *capacity, size_t *argv_idx, char *expanded, t_shell *shell)
{
	t_wildcard	*wild;
	size_t		match_count;
	char		**new_argv;

	wild = expand_wildcard(expanded, shell->pwd);
	match_count = count_wildcard_matches(wild);
	if (*argv_idx + match_count > *capacity)
	{
		*capacity = *argv_idx + match_count;
		new_argv = realloc(argv, sizeof(char *) * (*capacity + 1));
		if (!new_argv)
		{
			free_wildcard_list(wild);
			free(expanded);
			return (NULL);
		}
		argv = new_argv;
	}
	fill_wildcard_results(argv, argv_idx, wild);
	free_wildcard_list(wild);
	free(expanded);
	return (argv);
}

