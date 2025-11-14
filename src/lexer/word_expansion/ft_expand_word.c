/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/14 11:21:18 by ymizuniw         ###   ########.fr       */
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
			if (strchr(w->word, '$'))
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

char	**ft_expand_word(t_token **tokens, size_t token_count, t_shell *shell)
{
	char	***token_results;
	size_t	*token_result_counts;
	size_t	total_count;
	char	**final_res;
	size_t	i;
	size_t	j;
	size_t	k;
	t_word	*word_list;
	size_t	addition;
	size_t	value_len;

	// Allocate arrays to hold results for each token
	token_results = (char ***)xcalloc(sizeof(char **) * token_count);
	token_result_counts = (size_t *)xcalloc(sizeof(size_t) * token_count);
	if (!token_results || !token_result_counts)
		return (xfree(token_results), xfree(token_result_counts), NULL);
	// Process each token
	total_count = 0;
	i = 0;
	while (i < token_count)
	{
		if (!tokens[i] || !tokens[i]->value)
		{
			token_results[i] = (char **)xcalloc(sizeof(char *) * 2);
			if (!token_results[i])
				goto cleanup_error;
			token_results[i][0] = ft_strdup("");
			token_results[i][1] = NULL;
			token_result_counts[i] = 1;
			total_count += 1;
			i++;
			continue ;
		}
		value_len = ft_strlen(tokens[i]->value);
		word_list = gen_word(tokens[i]->value, value_len, &addition);
		if (!word_list)
			goto cleanup_error;
		// Check if token has wildcard to expand
		if (has_wildcard_to_expand(word_list))
		{
			token_results[i] = expand_token_with_wildcard(word_list, shell,
					&token_result_counts[i]);
			free_word_list(word_list);
			if (!token_results[i])
				goto cleanup_error;
		}
		else
		{
			token_results[i] = (char **)xcalloc(sizeof(char *) * 2);
			if (!token_results[i])
			{
				free_word_list(word_list);
				goto cleanup_error;
			}
			token_results[i][0] = expand_token_words(word_list, shell);
			token_results[i][1] = NULL;
			free_word_list(word_list);
			if (!token_results[i][0])
				goto cleanup_error;
			token_result_counts[i] = 1;
		}
		total_count += token_result_counts[i];
		i++;
	}
	// Combine all results into final array
	final_res = (char **)xcalloc(sizeof(char *) * (total_count + 1));
	if (!final_res)
		goto cleanup_error;
	k = 0;
	i = 0;
	while (i < token_count)
	{
		j = 0;
		while (j < token_result_counts[i])
		{
			final_res[k++] = ft_strdup(token_results[i][j]);
			if (!final_res[k - 1])
			{
				free_double_array(final_res);
				goto cleanup_error;
			}
			j++;
		}
		i++;
	}
	final_res[k] = NULL;
	// Cleanup temporary arrays
	i = 0;
	while (i < token_count)
	{
		if (token_results[i])
			free_double_array(token_results[i]);
		i++;
	}
	xfree(token_results);
	xfree(token_result_counts);
	return (final_res);
cleanup_error:
	i = 0;
	while (i < token_count)
	{
		if (token_results[i])
			free_double_array(token_results[i]);
		i++;
	}
	xfree(token_results);
	xfree(token_result_counts);
	return (NULL);
}
