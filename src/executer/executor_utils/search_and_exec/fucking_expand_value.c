#include "../../../../includes/minishell.h"

int	before_doller_cat(char **expanded_word, size_t expanded_word_len,
		char *value, size_t value_len)
{
	char	*tmp;

	tmp = realloc(*expanded_word, expanded_word_len + value_len + 1);
	if (!tmp)
		return (-1);
	*expanded_word = tmp;
	memcpy(*expanded_word + expanded_word_len, value, value_len);
	(*expanded_word)[expanded_word_len + value_len] = '\0';
	return (1);
}

int	doller_cat(char **doller, char **expanded_word, size_t expanded_word_len,
		t_shell *shell)
{
	size_t	value_idx;
	char	*key;
	char	*value;
	size_t	value_len;
	t_env	*env_entry;
	bool	need_free_value;
	char	*tmp;

	value_idx = 0;
	need_free_value = false;
	if ((*doller)[1] == '\0' || (!isalnum((unsigned char)(*doller)[1])
			&& (*doller)[1] != '_' && (*doller)[1] != '?'))
	{
		if (before_doller_cat(expanded_word, expanded_word_len, "$", 1) < 0)
			return (-1);
		*doller = *doller + 1;
		return (1);
	}
	if ((*doller)[1] == '?')
		value_idx = 1;
	else
		while ((*doller)[1 + value_idx] && (isalnum((unsigned char)(*doller)[1
					+ value_idx]) || (*doller)[1 + value_idx] == '_'))
			value_idx++;
	key = strndup(*doller + 1, value_idx);
	if (!key)
		return (-1);
	if (strcmp(key, "?") == 0)
	{
		value = ft_itoa(shell->last_exit_status);
		need_free_value = true;
	}
	else
	{
		env_entry = find_env(shell->env_list, key);
		value = env_entry ? env_entry->value : (char *)"";
	}
	free(key);
	value_len = strlen(value);
	tmp = realloc(*expanded_word, expanded_word_len + value_len + 1);
	if (!tmp)
	{
		if (need_free_value)
			free(value);
		return (-1);
	}
	*expanded_word = tmp;
	memcpy(*expanded_word + expanded_word_len, value, value_len);
	(*expanded_word)[expanded_word_len + value_len] = '\0';
	if (need_free_value)
		free(value);
	*doller = *doller + 1 + value_idx;
	return (1);
}

char	*expand_word(char *word, t_shell *shell)
{
	char	*expanded;
	char	*d;
	size_t	exp_len;
	size_t	plain_len;
	size_t	tail_len;

	if (!word)
		return (strdup(""));
	expanded = NULL;
	d = strchr(word, '$');
	exp_len = 0;
	while (d)
	{
		plain_len = (size_t)(d - word);
		if (plain_len && before_doller_cat(&expanded, exp_len, word,
				plain_len) < 0)
			return (NULL);
		exp_len += plain_len;
		if (doller_cat(&d, &expanded, exp_len, shell) < 0)
			return (NULL);
		exp_len = strlen(expanded);
		word = d;
		d = strchr(word, '$');
	}
	if (word && *word)
	{
		tail_len = strlen(word);
		if (before_doller_cat(&expanded, exp_len, word, tail_len) < 0)
			return (NULL);
	}
	return (expanded ? expanded : strdup(""));
}

char	**gen_argv(t_argv *argv_list, t_shell *shell)
{
	t_argv	*cur_argv;
	size_t	argv_idx;
	size_t	list_len;
	char	**argv;

	argv_idx = 0;
	argv = NULL;
	if (argv_list == NULL)
		return (NULL);
	list_len = 0;
	cur_argv = argv_list;
	while (cur_argv)
	{
		list_len++;
		cur_argv = cur_argv->next;
	}
	argv = malloc(sizeof(char *) * (list_len + 1));
	if (!argv)
		return (NULL);
	cur_argv = argv_list;
	argv_idx = list_len;
	while (cur_argv != NULL)
	{
		argv_idx--;
		if (cur_argv->to_expand == true)
			argv[argv_idx] = expand_word(cur_argv->word, shell);
		else
			argv[argv_idx] = strdup(cur_argv->word);
		cur_argv = cur_argv->next;
	}
	argv[list_len] = NULL;
	return (argv);
}
