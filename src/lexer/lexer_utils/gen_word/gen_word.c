#include "../../../../includes/minishell.h"

char			**ft_expand_word(t_word *word_list, t_shell *shell);
char			*expand_doller(t_word *word, t_shell *shell);
char			**expand_wildcard(const char *pattern, const char *path,
					size_t *wildcard_count);
t_word			*append_node(t_word *head, t_word *new);
size_t			quote_close_place(char quote, char *value, size_t i);
char			**expand_wildcard_word(t_word *word, char **res, size_t *i,
					size_t *word_list_count);
char			*expand_dollar_word(t_word *word, t_shell *shell);

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

static char	*expand_plain_word(t_word *word)
{
	assert(word->word != NULL);
	return (strdup(word->word));
}

static char	**expand_dispatch(t_expand_word *exp)
{
	if (exp->word->to_expand_doller)
	{
		exp->res[exp->i] = expand_dollar_word(exp->word, exp->shell);
		if (!exp->res[exp->i])
			return (free_double_array(exp->res), NULL);
		exp->i++;
	}
	else if (exp->word->to_expand_wildcard)
		return (expand_wildcard_word(exp->word, exp->res, &exp->i,
				&exp->word_list_count));
	else
	{
		exp->res[exp->i] = expand_plain_word(exp->word);
		if (!exp->res[exp->i])
			return (free_double_array(exp->res), NULL);
		exp->i++;
	}
	return (exp->res);
}

char	**ft_expand_word(t_word *wd_list, t_shell *shell)
{
	t_expand_word	exp;

	exp.word = wd_list;
	exp.i = 0;
	exp.word_list_count = count_word_list(wd_list);
	exp.res = (char **)xcalloc(sizeof(char *) * (exp.word_list_count + 1));
	exp.shell = shell;
	if (!exp.res)
		return (NULL);
	while (exp.i < exp.word_list_count && exp.word)
	{
		exp.res = expand_dispatch(&exp);
		if (!exp.res)
			return (NULL);
		exp.word = exp.word->next;
	}
	exp.res[exp.i] = NULL;
	return (exp.res);
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

// Generate word tokens from value string
t_word	*gen_word(char *value, size_t value_len, size_t *addition)
{
	size_t	i;
	char	quote;
	size_t	start;
	t_word	*head;
	t_word	*word;
	size_t	close_place;

	i = 0;
	quote = is_quote(value[i]);
	start = i;
	head = NULL;
	while (i < value_len)
	{
		word = (t_word *)xcalloc(sizeof(t_word));
		if (word == NULL)
			return (NULL);
		word->word = NULL;
		close_place = 0;
		if (quote != '\0')
			close_place = quote_close_place(quote, value, ++i);
		if (quote != '\0' && close_place > 0)
		{
			if (quote == '\'')
				word->to_expand_doller = false;
			else
				word->to_expand_doller = true;
			word->to_expand_wildcard = false;
			word->word = ext_unit(value, start, close_place);
			if (!word->word)
				return (free_word_list(head), xfree(word), NULL);
			i = close_place + 1;
		}
		else
		{
			if (value[i] == '$')
			{
				word->type = WD_DOLLER;
				word->to_expand_doller = true;
				i++;
			}
			else
				word->type = WD_LITERAL;
			while (i < value_len && !is_quote(value[i]) && value[i] != '$')
			{
				if (word->type == WD_DOLLER && value[i] == '*')
					break ;
				if (word->type != WD_DOLLER && value[i] == '*')
					word->to_expand_wildcard = true;
				i++;
			}
			word->word = ext_unit(value, start, i);
			if (!word->word)
				return (free_word_list(head), xfree(word), NULL);
		}
		head = append_node(head, word);
	}
	*addition = i;
	return (head);
}
