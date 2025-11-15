#ifndef WILDCARD_H
# define WILDCARD_H

# include "minishell.h"

bool	match_prefix(const char *name, const char *prefix, size_t *start);
bool	match_middle(const char *name, char **keys, size_t key_count,
			size_t *start);
bool	match_suffix(const char *name, const char *last);
bool	match_to_keys(char **keys, const char *name, const char *pattern);
char	**expand_wildcard(const char *pattern, const char *path,
			size_t *wildcard_count);
#endif