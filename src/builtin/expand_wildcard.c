#include "../../includes/minishell.h"

char	**expand_wildcard(const char *pattern, const char *path, size_t *wildcard_count)
{
	DIR				*dirp;
	struct dirent	*dent;
	char			**keys=NULL;
	char			**matches;
	size_t			count;
	size_t			cap;
	size_t			i;
	int				only_wild;

	dirp = opendir(path);
	if (!dirp)
		return (NULL);
	printf("expand_wildcard\n");
	/* check if pattern consists of only '*' */
	only_wild = 1;
	for (i = 0; pattern[i] != '\0'; i++)
	{
		if (pattern[i] != '*')
		{
			only_wild = 0;
			break;
		}
	}
	printf("wild card for loop break\n");
	count = 0;
	cap = 16;
	matches = malloc(sizeof(char *) * cap);
	if (!matches)
	{
		closedir(dirp);
		return (NULL);
	}
	while ((dent = readdir(dirp)))
	{
		if (dent->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (only_wild || match_to_keys(keys, dent->d_name))
		{
			if (count + 1 >= cap)
			{
				cap *= 2;
				char **tmp = realloc(matches, sizeof(char *) * cap);
				if (!tmp)
					break;
				matches = tmp;
			}
			matches[count++] = strdup(dent->d_name);
		}
	}
	matches[count] = NULL;
	*wildcard_count = count;
	if (!only_wild)
		free_double_array(keys);
	closedir(dirp);
	return (matches);
}
