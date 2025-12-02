/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_with_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:29:33 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 18:36:36 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	**create_fallback_result(t_word *word_list, t_shell *shell)
{
	char	**result;

	result = (char **)xcalloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = expand_plain_word(word_list, shell);
	if (!result[0])
	{
		free_double_array(result);
		return (NULL);
	}
	result[1] = NULL;
	return (result);
}

char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
		size_t *result_count)
{
	char	*pattern;
	char	**wildcard_results;
	size_t	count;

	pattern = expand_plain_word(word_list, shell);
	if (!pattern)
		return (NULL);
	wildcard_results = expand_wildcard(pattern, "./", &count);
	xfree(pattern);
	if (!wildcard_results || count == 0)
	{
		if (wildcard_results)
			free_double_array(wildcard_results);
		*result_count = 1;
		return (create_fallback_result(word_list, shell));
	}
	*result_count = count;
	return (wildcard_results);
}
