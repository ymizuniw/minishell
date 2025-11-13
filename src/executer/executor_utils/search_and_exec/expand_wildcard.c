#include "../../includes/minishell.h"

static int	check_only_wildcards(const char *pattern)
{
	size_t	i;

	i = 0;
	while (pattern[i] != '\0')
	{
		if (pattern[i] != '*')
			return (0);
		i++;
	}
	return (1);
}

static char	**init_matches_array(size_t *cap)
{
	char	**matches;

	*cap = 16;
	matches = malloc(sizeof(char *) * (*cap));
	return (matches);
}

static int	add_match(char ***matches, size_t *count, size_t *cap, char *name)
{
	char	**tmp;
	size_t	old_size;

	if (*count + 1 >= *cap)
	{
		old_size = sizeof(char *) * (*cap);
		*cap *= 2;
		tmp = ft_realloc(*matches, old_size, sizeof(char *) * (*cap));
		if (!tmp)
			return (-1);
		*matches = tmp;
	}
	(*matches)[*count] = ft_strdup(name);
	(*count)++;
	return (1);
}

static void	process_directory_entries(DIR *dirp, const char *pattern,
		t_match_data *data)
{
	struct dirent	*dent;
	char			*name;

	dent = readdir(dirp);
	while (dent)
	{
		name = dent->d_name;
		if (name[0] == '.' && pattern[0] != '.')
		{
			dent = readdir(dirp);
			continue ;
		}
		if (data->only_wild || match_to_keys(data->keys, name, pattern))
			if (add_match(&data->matches, &data->count, &data->cap, name) < 0)
				break ;
		dent = readdir(dirp);
	}
}

char	**expand_wildcard(const char *pattern, const char *path,
		size_t *wildcard_count)
{
	DIR				*dirp;
	t_match_data	data;

	dirp = opendir(path);
	if (!dirp)
		return (NULL);
	data.only_wild = check_only_wildcards(pattern);
	if (!data.only_wild)
		data.keys = ft_split(pattern, '*');
	else
		data.keys = NULL;
	if (!data.only_wild && !data.keys)
		return (closedir(dirp), NULL);
	data.matches = init_matches_array(&data.cap);
	if (!data.matches)
		return (closedir(dirp), NULL);
	data.count = 0;
	process_directory_entries(dirp, pattern, &data);
	data.matches[data.count] = NULL;
	*wildcard_count = data.count;
	if (!data.only_wild)
		free_double_array(data.keys);
	closedir(dirp);
	return (data.matches);
}
//このファイルbuiltinにもある。