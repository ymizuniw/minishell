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

	if (*count + 1 >= *cap)
	{
		*cap *= 2;
		tmp = realloc(*matches, sizeof(char *) * (*cap));
		if (!tmp)
			return (-1);
		*matches = tmp;
	}
	(*matches)[*count] = strdup(name);
	(*count)++;
	return (1);
}

char	**expand_wildcard(const char *pattern, const char *path,
		size_t *wildcard_count)
{
	DIR				*dirp;
	struct dirent	*dent;
	char			**keys;
	char			**matches;
	size_t			count;
	size_t			cap;
	int				only_wild;

	keys = NULL;
	dirp = opendir(path);
	if (!dirp)
		return (NULL);
	only_wild = check_only_wildcards(pattern);
	matches = init_matches_array(&cap);
	if (!matches)
		return (closedir(dirp), NULL);
	count = 0;
	while ((dent = readdir(dirp)))
	{
		if (dent->d_name[0] == '.' && pattern[0] != '.')
			continue ;
		if (only_wild || match_to_keys(keys, dent->d_name))
			if (add_match(&matches, &count, &cap, dent->d_name) < 0)
				break ;
	}
	matches[count] = NULL;
	*wildcard_count = count;
	if (!only_wild)
		free_double_array(keys);
	return (closedir(dirp), matches);
}
