#include "../../../../includes/minishell.h"

// Get variable value (handles $? and env vars)
static char	*get_var_value(const char *key, t_shell *shell, bool *need_free)
{
	t_env	*env_entry;

	if (ft_strcmp(key, "?") == 0)
	{
		*need_free = true;
		return (ft_itoa(shell->last_exit_status));
	}
	env_entry = find_env(shell->env_list, key);
	*need_free = false;
	if (env_entry)
		return (env_entry->value);
	return ("");
}

// Process single dollar expansion

static size_t	get_var_key_len(char *word)
{
	size_t	key_len;

	if (word[1] == '?')
		return (1);
	key_len = 0;
	while (ft_isalnum(word[1 + key_len]) || word[1 + key_len] == '_')
		key_len++;
	return (key_len);
}

int	expand_single_dollar(char **word_ptr, char **expanded, size_t *len,
		t_shell *shell)
{
	char	*key;
	char	*value;
	bool	need_free;
	size_t	key_len;
	int		ret;

	if (!(*word_ptr)[1] || (!ft_isalnum((*word_ptr)[1]) && (*word_ptr)[1] != '_'
			&& (*word_ptr)[1] != '?'))
		return (append_to_expanded(expanded, len, "$", 1), (*word_ptr)++, 1);
	key_len = get_var_key_len(*word_ptr);
	key = ft_strndup(*word_ptr + 1, key_len);
	if (!key)
		return (-1);
	value = get_var_value(key, shell, &need_free);
	free(key);
	ret = 0;
	if (value)
		ret = append_to_expanded(expanded, len, value, ft_strlen(value));
	else
		ret = append_to_expanded(expanded, len, value, 0);
	if (need_free)
		free(value);
	*word_ptr += 1 + key_len;
	return (ret);
}
