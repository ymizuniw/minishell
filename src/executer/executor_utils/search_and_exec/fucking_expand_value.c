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

static char	*get_var_value(char *key, t_shell *shell, bool *need_free)
{
	t_env	*env_entry;
	char	*value;

	if (strcmp(key, "?") == 0)
	{
		value = ft_itoa(shell->last_exit_status);
		*need_free = true;
	}
	else
	{
		env_entry = find_env(shell->env_list, key);
		if (env_entry)
			value = env_entry->value;
		else
			value = (char *)"";
		*need_free = false;
	}
	return (value);
}

static int	append_var_value(char **expanded_word, size_t expanded_word_len,
		char *value, bool need_free)
{
	char	*tmp;
	size_t	value_len;

	value_len = 0;
	if (value)
		value_len = strlen(value);
	tmp = realloc(*expanded_word, expanded_word_len + value_len + 1);
	if (!tmp)
	{
		if (need_free)
			xfree(value);
		return (-1);
	}
	*expanded_word = tmp;
	memcpy(*expanded_word + expanded_word_len, value, value_len);
	(*expanded_word)[expanded_word_len + value_len] = '\0';
	if (need_free)
		xfree(value);
	return (1);
}

int	doller_cat(char **doller, char **expanded_word, size_t expanded_word_len,
		t_shell *shell)
{
	size_t	value_idx;
	char	*key;
	char	*value;
	bool	need_free_value;

	value_idx = 0;
	if ((*doller)[1] == '\0' || (!isalnum((unsigned char)(*doller)[1])
			&& (*doller)[1] != '_' && (*doller)[1] != '?'))
		return (before_doller_cat(expanded_word, expanded_word_len, "$", 1),
			*doller = *doller + 1, 1);
	if ((*doller)[1] == '?')
		value_idx = 1;
	else
		while ((*doller)[1 + value_idx] && (isalnum((unsigned char)(*doller)[1
					+ value_idx]) || (*doller)[1 + value_idx] == '_'))
			value_idx++;
	key = strndup(*doller + 1, value_idx);
	if (!key)
		return (-1);
	value = get_var_value(key, shell, &need_free_value);
	xfree(key);
	if (append_var_value(expanded_word, expanded_word_len, value,
			need_free_value) < 0)
		return (-1);
	*doller = *doller + 1 + value_idx;
	return (1);
}

static int	expand_dollars(char **expanded, char **word, size_t *exp_len,
		t_shell *shell)
{
	size_t	plain_len;
	char	*d;

	d = strchr(*word, '$');
	while (d)
	{
		plain_len = (size_t)(d - *word);
		if (plain_len && before_doller_cat(expanded, *exp_len, *word,
				plain_len) < 0)
			return (-1);
		*exp_len += plain_len;
		if (doller_cat(&d, expanded, *exp_len, shell) < 0)
			return (-1);
		if (*expanded)
			*exp_len = strlen(*expanded);
		*word = d;
		d = strchr(*word, '$');
	}
	return (1);
}

char	*expand_word(char *word, t_shell *shell)
{
	char	*expanded;
	size_t	exp_len;
	size_t	tail_len;

	tail_len = 0;
	if (!word)
		return (strdup(""));
	expanded = NULL;
	exp_len = 0;
	if (expand_dollars(&expanded, &word, &exp_len, shell) < 0)
		return (NULL);
	if (word && *word)
	{
		if (word)
			tail_len = strlen(word);
		if (before_doller_cat(&expanded, exp_len, word, tail_len) < 0)
			return (NULL);
	}
	if (expanded)
		return (expanded);
	return (strdup(""));
}

// static size_t	count_argv_list(t_argv *argv_list)
// {
// 	t_argv	*cur;
// 	size_t	count;

// 	count = 0;
// 	cur = argv_list;
// 	while (cur)
// 	{
// 		count++;
// 		cur = cur->next;
// 	}
// 	return (count);
// }

// static void	fill_argv_array(char **argv, t_argv *argv_list, size_t list_len,
// 		t_shell *shell)
// {
// 	t_argv	*cur_argv;
// 	size_t	argv_idx;

// 	cur_argv = argv_list;
// 	argv_idx = list_len;
// 	while (cur_argv != NULL)
// 	{
// 		argv_idx--;
// 		if (cur_argv->to_expand == true)
// 			argv[argv_idx] = expand_word(cur_argv->word, shell);
// 		else
// 			argv[argv_idx] = strdup(cur_argv->word);
// 		cur_argv = cur_argv->next;
// 	}
// 	argv[list_len] = NULL;
// }

// char	**gen_argv(t_argv *argv_list, t_shell *shell)
// {
// 	size_t	list_len;
// 	char	**argv;

// 	if (argv_list == NULL)
// 		return (NULL);
// 	list_len = count_argv_list(argv_list);
// 	argv = xmalloc(sizeof(char *) * (list_len + 1));
// 	if (!argv)
// 		return (NULL);
// 	fill_argv_array(argv, argv_list, list_len, shell);
// 	return (argv);
// }
