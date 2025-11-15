/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_wildcard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 11:35:27 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*expand_token_words(t_word *word_list, t_shell *shell);
char	**expand_wildcard(const char *pattern, const char *path,
			size_t *wildcard_count);

// Check if word list contains wildcard to expand
bool	has_wildcard_to_expand(t_word *word_list)
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
char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
		size_t *result_count)
{
	char	*pattern;
	char	**wildcard_results;
	size_t	count;

	(void)shell;
	pattern = expand_token_words(word_list, shell);
	if (!pattern)
		return (NULL);
	wildcard_results = expand_wildcard(pattern, "./", &count);
	xfree(pattern);
	if (!wildcard_results || count == 0)
	{
		if (wildcard_results)
			free_double_array(wildcard_results);
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
