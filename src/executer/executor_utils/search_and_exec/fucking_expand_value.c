#include "../../../../includes/minishell.h"

// Simplified helper to append string to expanded word
static int	append_to_expanded(char **expanded, size_t *len, const char *str,
		size_t str_len)
{
	char	*tmp;

	tmp = realloc(*expanded, *len + str_len + 1);
	if (!tmp)
		return (-1);
	*expanded = tmp;
	memcpy(*expanded + *len, str, str_len);
	*len += str_len;
	(*expanded)[*len] = '\0';
	return (1);
}

// Get variable value (handles $? and env vars)
static char	*get_var_value(const char *key, t_shell *shell, bool *need_free)
{
	t_env	*env_entry;

	if (strcmp(key, "?") == 0)
	{
		*need_free = true;
		return (ft_itoa(shell->last_exit_status));
	}
	env_entry = find_env(shell->env_list, key);
	*need_free = false;
	return (env_entry ? env_entry->value : "");
}

// Process single dollar expansion
static int	expand_single_dollar(char **word_ptr, char **expanded, size_t *len,
		t_shell *shell)
{
	char	*key;
	char	*value;
	bool	need_free;
	size_t	key_len;
	int		ret;

	if (!(*word_ptr)[1] || (!isalnum((*word_ptr)[1]) && (*word_ptr)[1] != '_'
			&& (*word_ptr)[1] != '?'))
		return (append_to_expanded(expanded, len, "$", 1), (*word_ptr)++, 1);
	key_len = ((*word_ptr)[1] == '?') ? 1 : 0;
	while (key_len == 0 && (isalnum((*word_ptr)[1 + key_len])
			|| (*word_ptr)[1 + key_len] == '_'))
		key_len++;
	key = strndup(*word_ptr + 1, key_len);
	if (!key)
		return (-1);
	value = get_var_value(key, shell, &need_free);
	free(key);
	ret = append_to_expanded(expanded, len, value, value ? strlen(value) : 0);
	if (need_free)
		free(value);
	*word_ptr += 1 + key_len;
	return (ret);
}

// Main expansion function
char	*expand_word(char *word, t_shell *shell)
{
	char	*expanded;
	char	*dollar;
	char	*current;
	size_t	len;

	if (!word)
		return (strdup(""));
	expanded = NULL;
	len = 0;
	current = word;
	while ((dollar = strchr(current, '$')))
	{
		if (dollar > current && append_to_expanded(&expanded, &len, current,
				dollar - current) < 0)
			return (free(expanded), NULL);
		if (expand_single_dollar(&dollar, &expanded, &len, shell) < 0)
			return (free(expanded), NULL);
		current = dollar;
	}
	if (*current && append_to_expanded(&expanded, &len, current,
			strlen(current)) < 0)
		return (free(expanded), NULL);
	return (expanded ? expanded : strdup(""));
}
