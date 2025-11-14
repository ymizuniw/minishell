/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/14 20:04:58 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Forward declarations for internal word generation
t_word		*gen_word_from_token(t_token *token);
t_word		*gen_word(char *value, size_t value_len, size_t *addition);
int			expand_single_dollar(char **word_ptr, char **expanded, size_t *len,
				t_shell *shell);
int			append_to_expanded(char **expanded, size_t *len, const char *str,
				size_t str_len);
char		**expand_wildcard(const char *pattern, const char *path,
				size_t *wildcard_count);

// Expand string with dollar variables (for double-quoted strings)
static char	*expand_string_with_dollars(char *str, t_shell *shell)
{
	char	*result;
	char	*ptr;
	size_t	len;

	result = NULL;
	len = 0;
	ptr = str;
	while (*ptr)
	{
		if (*ptr == '$')
		{
			if (expand_single_dollar(&ptr, &result, &len, shell) < 0)
				return (xfree(result), NULL);
		}
		else
		{
			if (!append_to_expanded(&result, &len, ptr, 1))
				return (xfree(result), NULL);
			ptr++;
		}
	}
	return (result);
}

// Expand and concatenate all t_word nodes from a single token into one string
static char	*expand_token_words(t_word *word_list, t_shell *shell)
{
	char	*result;
	char	*temp;
	char	*expanded;
	t_word	*w;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	w = word_list;
	while (w)
	{
		if (w->to_expand_doller && w->type == WD_DOLLER)
		{
			// Check if this is a double-quoted string with $ inside
			if (ft_strchr(w->word, '$'))
				expanded = expand_string_with_dollars(w->word, shell);
			else
				expanded = expand_doller(w, shell);
		}
		else
			expanded = ft_strdup(w->word);
		if (!expanded)
			return (xfree(result), NULL);
		temp = result;
		result = ft_strjoin(temp, expanded);
		xfree(temp);
		xfree(expanded);
		if (!result)
			return (NULL);
		w = w->next;
	}
	return (result);
}

// Check if word list contains wildcard to expand
static bool	has_wildcard_to_expand(t_word *word_list)
{
	t_word	*w;

	w = word_list;
	while (w)
	{
		if (w->to_expand_wildcard)
			return (true);
		w = w->next;
	}
	return (false);
}

// Expand token with wildcard - returns array of expanded strings
static char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
		size_t *result_count)
{
	char	*pattern;
	char	**wildcard_results;
	size_t	count;

	(void)shell;
	// First, expand any dollar variables to get the final pattern
	pattern = expand_token_words(word_list, shell);
	if (!pattern)
		return (NULL);
	// Then expand wildcard
	wildcard_results = expand_wildcard(pattern, "./", &count);
	xfree(pattern);
	if (!wildcard_results || count == 0)
	{
		if (wildcard_results)
			free_double_array(wildcard_results);
		// No matches, return the pattern as-is
		wildcard_results = (char **)xcalloc(sizeof(char *) * 2);
		if (!wildcard_results)
			return (NULL);
		wildcard_results[0] = expand_token_words(word_list, shell);
		if (!wildcard_results[0])
		{
			free_double_array(wildcard_results);
			return (NULL);
		}
		wildcard_results[1] = NULL;
		*result_count = 1;
		return (wildcard_results);
	}
	*result_count = count;
	return (wildcard_results);
}

//t_token_result ,,,
void cleanup_token_results(size_t token_count, char ***token_results, size_t *token_result_counts)
{
	size_t i;
	i = 0;
	while (i < token_count)
	{
		if (token_results[i])
			free_double_array(token_results[i]);
		i++;
	}
	xfree(token_results);
	xfree(token_result_counts);
}

typedef struct s_token_result
{
	char	***token_results;
	size_t	*token_result_counts;
	size_t	total_count;
	size_t	i;
} t_token_result;

// Allocate arrays to hold results for each token
int init_token_results(t_token_result *tr, size_t token_count)
{
	//tr->token_results is triple-ptr, and the necessary ptr depth is double for execve().
	//then the parent ptr shall be free() ed. when the elements are delegated to execve().
	//
	tr->token_results = (char ***)xcalloc(sizeof(char **) * token_count);//the list of char *argv[], {char *argv[], ...};
	tr->token_result_counts = (size_t *)xcalloc(sizeof(size_t) * token_count);//the size of each char *argv[] corresponding to the idx of token_results.
	if (!tr->token_results || !tr->token_result_counts)
		return (xfree(tr->token_results), xfree(tr->token_result_counts), -1);
	return (1);
}

void init_token_results_elem(t_token_result *tr, t_token **tokens, size_t token_count)
{
	while (tr->i < token_count)
	{
		if (!tokens[tr->i] && !tokens[tr->i]->value)
		{
			tr->token_results[tr->i] = (char **)xcalloc(sizeof(char *) * 2);
			if (!tr->token_results[tr->i])
			{
				cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
				return ;
			}
			tr->token_results[tr->i][0] = ft_strdup("");
			tr->token_results[tr->i][1] = NULL;
			tr->token_result_counts[tr->i] = 1;
			tr->total_count += 1;
			(tr->i)++;
			continue ;
		}
		else
			break;
	}
}

int with_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list, size_t token_count)
{
	tr->token_results[tr->i] = expand_token_with_wildcard(word_list, shell,
			&tr->token_result_counts[tr->i]);
	free_word_list(word_list);
	if (!tr->token_results[tr->i])
	{
		cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
		return (-1);
	}
	return (1);
}

int without_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list, size_t token_count)
{
	tr->token_results[tr->i] = (char **)xcalloc(sizeof(char *) * 2);
	if (!tr->token_results[tr->i])
	{
		free_word_list(word_list);
		cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
		return (-1);
	}
	tr->token_results[tr->i][0] = expand_token_words(word_list, shell);
	tr->token_results[tr->i][1] = NULL;
	free_word_list(word_list);
	if (!tr->token_results[tr->i][0])
	{
		cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
		return (-1);
	}
	tr->token_result_counts[tr->i] = 1;
	return (1);
}


//=================================================================
// ==2387728==ERROR: AddressSanitizer: stack-buffer-overflow
int build_final_res(char ***final_res, t_token_result *tr, size_t token_count)
{
	size_t k;
	size_t j;
	
	*final_res = (char **)xcalloc(sizeof(char *) * (tr->i + 1));

	//alloc memory size
	//loop count tr->i
	//token_results[tr->] 's NULL check
	/*
		it seems that final_res is no realloc()ed when new string is put in the array.
	*/
	
	if (!final_res)
	{
		cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
		return (-1);
	}
	k = 0;
	tr->i = 0;
	while (tr->i < token_count)
	{
		j = 0;
		while (j < tr->token_result_counts[tr->i])
		{
			(*final_res)[k++] = ft_strdup(tr->token_results[tr->i][j]);
			if (!final_res[k - 1])
			{
				// free_double_array(final_res[]);
				cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
				return (-1);					
			}
			j++;
		}
		tr->i++;
	}
	(*final_res)[k] = NULL;
	return (1);
}

int expand_loop(t_shell *shell, t_token_result *tr, t_token **tokens, size_t token_count)
{
	size_t value_len;
	size_t addition = 0;
	t_word *word_list = NULL;
	
	while (tr->i < token_count)
	{
		init_token_results_elem(tr, tokens, token_count);
		value_len = ft_strlen(tokens[tr->i]->value);
		word_list = gen_word(tokens[tr->i]->value, value_len, &addition);
		if (!word_list)
		{
			cleanup_token_results(token_count, tr->token_results, tr->token_result_counts);
			return (-1);			
		}
		if (has_wildcard_to_expand(word_list))//with wildcard path
		{
			if (with_wildcard(shell, tr, word_list, token_count)<0)
				return (cleanup_token_results(token_count, tr->token_results, tr->token_result_counts),-1);
		}
		else//without wildcard path
		{
			if (without_wildcard(shell, tr, word_list, token_count)<0)
				return (cleanup_token_results(token_count, tr->token_results, tr->token_result_counts),-1);
		}
		tr->total_count += tr->token_result_counts[tr->i];
		tr->i++;
	}
	return (1);
}

//t_token **tr->tokens is t_token *token 's address array.
char	**ft_expand_word(t_token **tokens, size_t token_count, t_shell *shell)
{
	t_token_result tr;
	char	**final_res;

	if (init_token_results(&tr, token_count)<0)
		return (NULL);
	tr.total_count = 0;
	tr.i = 0;
	if (expand_loop(shell, &tr, tokens, token_count)<0)
		return (NULL);
	if (build_final_res(&final_res, &tr, token_count)<0)
		return (NULL);
	cleanup_token_results(token_count, tr.token_results, tr.token_result_counts);
	return (final_res);
}
