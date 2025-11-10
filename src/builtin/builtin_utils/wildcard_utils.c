#include "../../../includes/minishell.h"

bool	match_prefix(const char *name, const char *prefix, size_t *start)
{
	size_t	prefix_len=0;

	if (!prefix || prefix[0] == '\0')
		return (true);
	prefix_len = strlen(prefix);
	if (strncmp(name, prefix, prefix_len) != 0)
		return (false);
	*start = prefix_len;
	return (true);
}

bool	match_middle(const char *name, char **keys, size_t key_count, size_t *start)
{
	size_t	i;
	char	*found;
	size_t key_len = 0;

	assert(keys!=NULL);
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
	size_t	name_len=0;
	size_t	last_len=0;

	assert(last!=NULL);
	printf("last addres: %p\n", last);
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

bool	match_to_keys(char **keys, const char *name)
{
	size_t	key_count;
	size_t	start;

	key_count = 0;
	start = 0;
	printf("key_count: %zu\n", key_count);
	printf("key: %s\n", keys[0]);
	while (keys[key_count])
		key_count++;
	if (key_count == 1)
		return (strcmp(keys[0], name) == 0);
	if (!match_prefix(name, keys[0], &start))
		return (false);
	if (!match_middle(name, keys, key_count, &start))
		return (false);
	printf("key_count: %zu\n", key_count);
	printf("key: %s\n", keys[0]);
	return (match_suffix(name, keys[key_count - 1]));
}
