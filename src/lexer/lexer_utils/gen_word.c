#include "../../../includes/minishell.h"

// Forward declarations for internal word generation
t_word		*append_node(t_word *head, t_word *new);
char		*expand_doller(t_word *word, t_shell *shell);
char		**expand_wildcard(const char *pattern, const char *path,
				size_t *wildcard_count);
size_t		quote_close_place(char quote, char *value, size_t i);
size_t		count_word_list(t_word *word);

// Extract a substring from src between start and end indices
char	*ext_unit(char *src, size_t start, size_t end)
{
	return (ft_substr(src, start, end - start));
}

// Join two strings by reallocating and concatenating
int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = ft_realloc(*res, sizeof(char) * (size1 + 1), sizeof(char) * (size1
				+ size2 + 1));
	if (*res == NULL)
		return (0);
	ft_memcpy(*res + size1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
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
	t_word	*word;
	size_t	value_len;
	size_t	start;
	char	quote;
	size_t	close_place;
}			t_gen_word;

// give start idx,
int	quote_wrapper(t_gen_word *gw, char *value, size_t *i)
{
	char	*content;

	if (gw->quote == '\'')
		gw->word->to_expand_doller = false;
	else
		gw->word->to_expand_doller = true;
	gw->word->to_expand_wildcard = false;
	content = ext_unit(value, gw->start + 1, gw->close_place);
	if (!content)
		return (-1);
	gw->word->word = content;
	// Set type based on content and quote type
	if (gw->quote == '"' && ft_strchr(content, '$'))
		gw->word->type = WD_DOLLER; // Contains $ in double quotes
	else if (gw->quote == '"' && content[0] == '$' && content[1] != '\0')
		gw->word->type = WD_DOLLER;
	else
		gw->word->type = WD_LITERAL;
	*i = gw->close_place + 1;
	return (1);
}

static int correct_env_var_format(char c)
{
	if (c == '?' || c == '*'
				|| c == '@' || c == '#' || c == '$'
				|| c == '!' || ft_isdigit(c))
		return (1);
	else
		return (0);
}

int	doller_literal_wrapper(t_gen_word *gw, char *value, size_t value_len,
		size_t *i)
{
	if (value && value[*i] == '$')
	{
		(*i)++;
		if (*i < value_len && correct_env_var_format(value[*i]))
		{
			gw->word->type = WD_DOLLER;
			gw->word->to_expand_doller = true;
			(*i)++;
		}
		// Regular variable names: alphanumeric and underscore
		else if (*i < value_len && (ft_isalpha(value[*i]) || value[*i] == '_'))
		{
			gw->word->type = WD_DOLLER;
			gw->word->to_expand_doller = true;
			while (*i < value_len && (ft_isalnum(value[*i])
					|| value[*i] == '_'))
				(*i)++;
		}
		// Lone $ with no valid variable name following - keep as literal $
		else
		{
			gw->word->type = WD_LITERAL;
			gw->word->to_expand_doller = false;
		}
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

int	init_gen_word_data(t_word **word, t_gen_word *gw, char *value, size_t *i)
{
	*word = (t_word *)xcalloc(sizeof(t_word));
	if (*word == NULL)
		return (-1);
	ft_memset(gw, 0, sizeof(t_gen_word));
	gw->quote = is_quote(value[*i]);
	gw->close_place = 0;
	gw->word = *word;
	return (1);
}

t_word	*loop_wrapper(char *value, size_t value_len, size_t *i)
{
	t_word		*word;
	t_gen_word	gw;

	if (init_gen_word_data(&word, &gw, value, i) < 0)
		return (NULL);
	gw.start = *i;
	if (gw.quote != '\0')
		gw.close_place = quote_close_place(gw.quote, value, ++(*i));
	if (gw.quote != '\0' && gw.close_place > 0)
	{
		if (quote_wrapper(&gw, value, i) < 0)
			return (xfree(word), NULL);
	}
	else
	{
		doller_literal_wrapper(&gw, value, value_len, i);
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
	t_word	*head;
	t_word	*word;
	t_word	*new_head;

	head = NULL;
	word = NULL;
	i = 0;
	while (i < value_len)
	{
		word = loop_wrapper(value, value_len, &i);
		if (word == NULL)
			return (free_word_list(head), NULL);
		new_head = append_node(head, word);
		if (!new_head && head != NULL)
			return (xfree(word), free_word_list(head), NULL);
		head = new_head;
	}
	*addition = i;
	return (head);
}
