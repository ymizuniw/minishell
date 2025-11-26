/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:10 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/27 02:34:14 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	match_prefix(const char *name, const char *prefix, size_t *start)
{
	size_t	prefix_len;

	if (!prefix || prefix[0] == '\0')
		return (true);
	prefix_len = ft_strlen(prefix);
	if (ft_strncmp(name, prefix, prefix_len) != 0)
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
	i = 1;
	while (i + 1 < key_count)
	{
		found = ft_strstr(name + *start, keys[i]);
		if (!found)
			return (false);
		if (keys[i])
			key_len = ft_strlen(keys[i]);
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
	if (name)
		name_len = ft_strlen(name);
	if (!last || last[0] == '\0')
		return (true);
	if (last)
		last_len = ft_strlen(last);
	if (name_len < last_len)
		return (false);
	return (ft_strcmp(name + name_len - last_len, last) == 0);
}

bool	match_single_key(const char *key, const char *name,
		bool starts_with_wildcard, bool ends_with_wildcard)
{
	if (starts_with_wildcard && ends_with_wildcard)
		return (ft_strstr(name, key) != NULL);
	if (starts_with_wildcard)
		return (match_suffix(name, key));
	if (ends_with_wildcard)
		return (match_prefix(name, key, &(size_t){0}));
	return (ft_strcmp(key, name) == 0);
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
		return (match_single_key(keys[0], name, starts_with_wildcard,
				ends_with_wildcard));
	}
	if (!match_prefix(name, keys[0], &start))
		return (false);
	if (!match_middle(name, keys, key_count, &start))
		return (false);
	return (match_suffix(name, keys[key_count - 1]));
}
