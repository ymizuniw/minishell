#include "../../../includes/minishell.h"

char			**ft_expand_word(t_word *word_list, t_shell *shell);
char			*expand_doller(t_word *word, t_shell *shell);
char			**expand_wildcard(const char *pattern, const char *path,
					size_t *wildcard_count);
t_word			*append_node(t_word *head, t_word *new);
size_t			quote_close_place(char quote, char *value, size_t i);

// Extract a substring from src between start and end indices
char	*ext_unit(char *src, size_t start, size_t end)
{
	char	*unit;
	size_t	len;

	len = end - start;
	unit = xmalloc(sizeof(char) * (len + 1));
	if (unit == NULL)
		return (NULL);
	strncpy(unit, &src[start], len);
	unit[len] = '\0';
	return (unit);
}

// Join two strings by reallocating and concatenating
int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = realloc(*res, sizeof(char) * (size1 + size2 + 1));
	if (*res == NULL)
		return (0);
	strncpy(*res + size1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
}

static size_t	count_word_list(t_word *word)
{
	size_t	size;

	size = 0;
	while (word)
	{
		size++;
		word = word->next;
	}
	return (size);
}

// Return value should not be freed separately
char	*expand_doller(t_word *word, t_shell *shell)
{
	char	*env_value;
	char	*target;
	char	exit_status_str[12];

	env_value = NULL;
	target = word->word;
	if (word->type == WD_DOLLER)
		target = &word->word[1];
	// Handle special variable $?
	if (strcmp(target, "?") == 0)
	{
		snprintf(exit_status_str, sizeof(exit_status_str), "%d",
			shell->last_exit_status);
		return (strdup(exit_status_str));
	}
	env_value = get_env_value(shell->env_list, target);
	if (!env_value)
		return (strdup(""));
	return (strdup(env_value));
}

char	**ft_expand_word(t_word *wd_list, t_shell *shell)
{
	t_word	*word;
	char	*plain_expand;
	char	*env_expand;
	char	**wildcard_arr;
	size_t	wildcard_count;
	size_t	wildcard_idx;
	char	**res;
	size_t	word_list_count;
	size_t	i;
			char **new_res;

	word = wd_list;
	plain_expand = NULL;
	env_expand = NULL;
	wildcard_arr = NULL;
	wildcard_count = 0;
	wildcard_idx = 0;
	res = NULL;
	word_list_count = 0;
	i = 0;
	word_list_count = count_word_list(wd_list);
	res = (char **)xcalloc(sizeof(char *) * (word_list_count + 1));
	if (!res)
		return (NULL);
	while (i < word_list_count && word)
	{
		if (word->to_expand_doller)
		{
			env_expand = expand_doller(word, shell);
			if (!env_expand)
				env_expand = strdup("");
			res[i] = env_expand;
			if (!res[i])
				return (free_double_array(res), NULL);
			i++;
		}
		else if (word->to_expand_wildcard)
		{
			wildcard_arr = expand_wildcard(word->word, "./", &wildcard_count);
			if (!wildcard_arr)
				return (free_double_array(res), NULL);
			new_res = realloc(res, sizeof(char *) * (word_list_count
						+ wildcard_count + 1));
			if (!new_res)
				return (free_double_array(res), free_double_array(wildcard_arr),
					NULL);
			res = new_res;
			wildcard_idx = 0;
			while (wildcard_idx < wildcard_count)
			{
				res[i] = strdup(wildcard_arr[wildcard_idx]);
				if (!res[i])
					return (free_double_array(res),
						free_double_array(wildcard_arr), NULL);
				i++;
				wildcard_idx++;
			}
			word_list_count += wildcard_count;
			free_double_array(wildcard_arr);
			wildcard_arr = NULL;
		}
		else
		{
			assert(word->word != NULL);
			plain_expand = strdup(word->word);
			if (!plain_expand)
				return (free_double_array(res), NULL);
			res[i] = plain_expand;
			i++;
		}
		word = word->next;
	}
	res[i] = NULL;
	return (res);
}

// Append a word node to the list
t_word	*append_node(t_word *head, t_word *new)
{
	t_word	*original_head;

	original_head = head;
	word_add_back(&head, new);
	if (original_head == NULL)
		return (new);
	return (original_head);
}

size_t	quote_close_place(char quote, char *value, size_t i)
{
	if (value == NULL)
		return (0);
	while (value[i] != '\0')
	{
		if (value[i] == quote)
			return (i);
		i++;
	}
	return (0);
}

	typedef struct s_gen_word
	{
		t_word *word;
		size_t value_len;
		size_t start;
		char quote;
		size_t close_place;
	} t_gen_word;

	//give start idx, 
		int quote_wrapper(t_gen_word *gw, char *value, size_t *i)
		{
			if (gw->quote == '\'')
				gw->word->to_expand_doller = false;
			else
				gw->word->to_expand_doller = true;
			gw->word->to_expand_wildcard = false;
			gw->word->word = ext_unit(value, gw->start+1, gw->close_place);
			if (!gw->word->word)
				return (-1);
			*i = gw->close_place + 1;
			return (-1);
		}

		int doller_literal_wrapper(t_gen_word *gw, char *value, size_t value_len, size_t *i)
		{
			assert(gw);
			assert(gw->word);
			if (value && value[*i] == '$')
			{
				gw->word->type = WD_DOLLER;
				gw->word->to_expand_doller = true;
				(*i)++;
			}
			else
			{
				gw->word->type = WD_LITERAL;
				while (*i < value_len && !is_quote(value[*i]) && value[*i] != '$')
				{
					if (gw->word->type == WD_DOLLER && value[*i] == '*')
						break ;
					if (gw->word->type != WD_DOLLER && value[*i] == '*')
						gw->word->to_expand_wildcard = true;
					(*i)++;
				}
			}
			return (1);
		}



	int init_gen_word_data(t_word **word, t_gen_word *gw, char *value, size_t *i)
	{
		*word = (t_word *)xcalloc(sizeof(t_word));
		if (*word == NULL)
			return (-1);
		memset(gw, 0, sizeof(t_gen_word));
		gw->quote = is_quote(value[*i]);
		gw->close_place = 0;
		gw->word=*word;
		return (1);
	}

t_word *loop_wrapper(char *value, size_t value_len, size_t *i)
{
	t_word	*word;
	t_gen_word gw;
	
	if(init_gen_word_data(&word, &gw, value, i)<0)
		return (NULL);
	gw.start = *i;
	if (gw.quote != '\0')
		gw.close_place = quote_close_place(gw.quote, value, ++(*i));
	if (gw.quote != '\0' && gw.close_place > 0)
	{
		if (quote_wrapper(&gw, value, i)<0)
			return (xfree(word),NULL);
	}
	else
	{
		doller_literal_wrapper(&gw, value,value_len,i);
		word->word = ext_unit(value, gw.start, *i);
		if (!word->word)
			return (xfree(word), NULL);
	}
	return (word);
}

// Generate word tokens from value string
t_word	*gen_word(char *value, size_t value_len, size_t *addition)
{
	size_t	i;
	t_word *head=NULL;
	t_word *word = NULL;

	i = 0;
	head = NULL;
	while (i < value_len)
	{
		word = loop_wrapper(value, value_len, &i);
		if (word==NULL)
			return (free_word_list(head), NULL);
		head = append_node(head, word);
	}
	*addition = i;
	return (head);
}
