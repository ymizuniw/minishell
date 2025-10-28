#include "../../../includes/minishell.h"

int handle_no_expansion(char **word, char const *value, size_t word_len_no_expansion)
{
	word = xmalloc(sizeof(char) * (word_len_no_expansion + 1));
	if (!word)
		return (-1);
	strncpy(word, token->value, word_len_no_expansion + 1);
	return (1);
}

int handle_doller_expansion(char const **word, char const *value, char *doller)
{
	size_t	i;
	char	*var;
	char	*entry;
	size_t	start;

	i=0;
	while (doller && doller[i])
	{
		start = i;//start point of searching.
		i++;//next char.
		while (doller[i] && !isspace(doller[i]) && doller[i] != '$')
			i++;//if space or $ is following '$', it is the end of $ expansion.
		var = xmalloc(sizeof(char)*(i-start+1));//malloc() the length of var to expand + null term.
		if (!var)
			return (-1);
		memset(var, 0, sizeof(char)*(i-start+1));
		strncpy(var, doller + 1, i - start);//copy after doller to var of the length of i - start.
		entry = genenv(var);//search env var from env_list. this will be changed for local env list.
		if (entry!=NULL)//if entry found,
			cat_word(word, entry, strlen(entry));
		xfree(var);
		doller = strchr(doller + 1, '$');
	}
	return (1);
}

int search_doller_and_expand(char **word, char const *value, size_t word_len_no_expansion)
{
	char	*word;
	char	*doller;

	doller = strchr(value, '$');
	if (doller != NULL)
	{
		if (handle_doller_expansion(word, value, doller)<0)
			return (-1);
	}
	else
	{
		if (handle_no_expansion(word, value, word_len_no_expansion)<0)
			return (-1);
	}
	return (1);
}

// just expand the value content or apply $ expansion and form a word.
char	*expand_value(t_token *token)
{
	size_t	word_len_no_expansion;
	char *word;

	//initialize
	word_len_no_expansion = strlen(token->value);
	word = NULL;
	if (token->in_squote || token->type == TK_DOLLER)
	{
		if (handle_no_expansion(&word, token->value, word_len_no_expansion)<0)
			return (NULL);
	}
	else
	{
		if (search_doller_and_expand(&word, token->value, word_len_no_expansion)<0)
			return (NULL);
	}
	return (word);
}
