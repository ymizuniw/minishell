#include "../../../../includes/minishell.h"

int	append_to_expanded(char **expanded, size_t *len, const char *str,
		size_t str_len)
{
	char	*tmp;
	size_t	old_size;

	old_size = *len + 1;
	tmp = ft_realloc(*expanded, old_size, *len + str_len + 1);
	if (!tmp)
		return (-1);
	*expanded = tmp;
	memcpy(*expanded + *len, str, str_len);
	*len += str_len;
	(*expanded)[*len] = '\0';
	return (1);
}

static int	dollar_expansions(char **expanded, size_t *len, char **current,
		t_shell *shell)
{
	char	*dollar;

	dollar = strchr(*current, '$');
	while (dollar)
	{
		if (dollar > *current && append_to_expanded(expanded, len, *current,
				dollar - *current) < 0)
			return (-1);
		if (expand_single_dollar(&dollar, expanded, len, shell) < 0)
			return (-1);
		*current = dollar;
	}
	return (0);
}

// Main expansion function
char	*expand_word(char *word, t_shell *shell)
{
	char	*expanded;
	char	*current;
	size_t	len;
	size_t	current_len;

	if (!word)
		return (ft_strdup(""));
	expanded = NULL;
	len = 0;
	current = word;
	if (dollar_expansions(&expanded, &len, &current, shell) < 0)
		return (free(expanded), NULL);
	current_len = 0;
	if (current)
		current_len = ft_strlen(current);
	if (*current && append_to_expanded(&expanded, &len, current,
			current_len) < 0)
		return (free(expanded), NULL);
	if (expanded)
		return (expanded);
	return (ft_strdup(""));
}
