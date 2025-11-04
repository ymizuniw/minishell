
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===========================
  構造体定義
===========================*/
typedef struct s_wildcard
{
	char				name[256];
	bool				expand;
	struct s_wildcard	*next;
}						t_wildcard;

void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	free_double_array(char **arr)
{
	if (!arr)
		return ;
	for (size_t i = 0; arr[i]; i++)
		free(arr[i]);
	free(arr);
}

char	**ft_split(const char *str, char c)
{
	size_t	len;
	size_t	count;
	size_t	idx;
	size_t	start;
	size_t	seg_len;
	char	*part;
	char	**res;

	if (!str)
		return (NULL);
	len = strlen(str);
	count = 1;
	for (size_t i = 0; i < len; i++)
		if (str[i] == c)
			count++;
	res = calloc(count + 2, sizeof(char *));
	if (!res)
		return (NULL);
	idx = 0;
	start = 0;
	for (size_t i = 0; i <= len; i++)
	{
		if (str[i] == c || str[i] == '\0')
		{
			seg_len = i - start;
			part = malloc(seg_len + 1);
			if (!part)
				return (NULL);
			memcpy(part, str + start, seg_len);
			part[seg_len] = '\0';
			res[idx++] = part;
			start = i + 1;
		}
	}
	res[idx] = NULL;
	return (res);
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

t_wildcard	*expand_wildcard(const char *pattern, const char *path)
{
	DIR				*dirp;
	t_wildcard		*head;
	struct dirent	*dent;
	t_wildcard		*new;

	dirp = opendir(path);
	if (!dirp)
	{
		perror("opendir");
		return (NULL);
	}
	head = xmalloc(sizeof(t_wildcard));
	memset(head, 0, sizeof(t_wildcard));
	while ((dent = readdir(dirp)) != NULL)
	{
		if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
			continue ;
		new = xmalloc(sizeof(t_wildcard));
		memset(new, 0, sizeof(t_wildcard));
		strncpy(new->name, dent->d_name, sizeof(new->name) - 1);
		new->expand = false;
		append_wildcard(head, new);
	}
	closedir(dirp);
	mark_wc_to_expand(pattern, head);
	return (head);
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

int	main(int argc, char **argv)
{
	t_wildcard	*list;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	list = expand_wildcard(argv[1], ".");
	if (!list)
		return (EXIT_FAILURE);
	printf("=== Match results for pattern \"%s\" ===\n", argv[1]);
	for (t_wildcard *cur = list->next; cur; cur = cur->next)
	{
		if (cur->expand)
			printf("[MATCH] %s\n", cur->name);
		else
			printf("        %s\n", cur->name);
	}
	free_wildcard_list(list);
	return (EXIT_SUCCESS);
}
