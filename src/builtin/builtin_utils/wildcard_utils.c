#include "../../../includes/minishell.h"

bool	match_prefix(const char *name, const char *prefix, size_t *start)
{
	size_t	prefix_len;

	prefix_len = 0;
	if (!prefix || prefix[0] == '\0')
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
	size_t	key_len;

	key_len = 0;
	assert(keys != NULL);
	i = 1;
	while (i + 1 < key_count)
	{
		found = strstr(name + *start, keys[i]);
		if (!found)
			return (false);
		if (keys[i])
			key_len = strlen(keys[i]);
		*start = (found - name) + key_len;
		i++;
	}
	return (true);
}

bool	match_suffix(const char *name, const char *last)
{
	size_t	name_len;
	size_t	last_len;

	name_len = 0;
	last_len = 0;
	assert(last != NULL);
	if (name)
		name_len = strlen(name);
	if (!last || last[0] == '\0')
		return (true);
	if (last)
		last_len = strlen(last);
	if (name_len < last_len)
		return (false);
	return (strcmp(name + name_len - last_len, last) == 0);
}

bool	match_to_keys(char **keys, const char *name, const char *pattern)
{
	size_t	key_count;
	size_t	start;
	bool	starts_with_wildcard;
	bool	ends_with_wildcard;

	key_count = 0;
	start = 0;
	while (keys[key_count])
		key_count++;
	if (key_count == 0)
		return (true);
	starts_with_wildcard = (pattern[0] == '*');
	ends_with_wildcard = (pattern[ft_strlen(pattern) - 1] == '*');
	if (key_count == 1)
	{
		if (starts_with_wildcard && ends_with_wildcard)
			return (strstr(name, keys[0]) != NULL);
		if (starts_with_wildcard)
			return (match_suffix(name, keys[0]));
		if (ends_with_wildcard)
			return (match_prefix(name, keys[0], &start));
		return (strcmp(keys[0], name) == 0);
	}
	if (!match_prefix(name, keys[0], &start))
		return (false);
	if (!match_middle(name, keys, key_count, &start))
		return (false);
	return (match_suffix(name, keys[key_count - 1]));
}
