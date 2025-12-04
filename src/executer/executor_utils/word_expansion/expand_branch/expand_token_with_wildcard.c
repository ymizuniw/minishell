/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token_with_wildcard.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:29:33 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/05 00:22:58 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

//quoted flag usage is right?
// static char	**create_fallback_result(t_word *word_list, t_shell *shell)
// {
// 	char	**result;

// 	result = (char **)xcalloc(sizeof(char *) * 2);
// 	if (!result)
// 		return (NULL);
// 	bool quoted = false;
// 	result[0] = expand_plain_word(word_list, shell, &quoted);
// 	if (!result[0])
// 	{
// 		free_double_array(result);
// 		return (NULL);
// 	}
// 	result[1] = NULL;
// 	return (result);
// }

// char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
// 		size_t *result_count)
// {
// 	char	*pattern;
// 	char	**wildcard_results;
// 	size_t	count;
// 	bool quoted=false;
// 	pattern = expand_plain_word(word_list, shell, &quoted);
// 	if (!pattern)
// 		return (NULL);
//     if (quoted)
//     {
// 		char **results=NULL;
//         results = xcalloc(sizeof(char *)*2);
//         if (!results)
//             return NULL;
//         results[0] = pattern;
//         results[1] = NULL;
//         *result_count = 1;
//         return results;
//     }
// 	wildcard_results = expand_wildcard(pattern, "./", &count);
// 	if (!wildcard_results || count == 0)
// 	{
// 		char **results = NULL;
// 		results = xcalloc(sizeof(char *)*2);
// 		results[0] = pattern;
// 		results[1] = NULL;
// 		xfree(pattern);
// 		return results;
// 		// if (wildcard_results)
// 		// 	free_double_array(wildcard_results);
// 		// *result_count = 1;
// 		// return (create_fallback_result(word_list, shell));
// 	}
// 	xfree(pattern);
// 	*result_count = count;
// 	return (wildcard_results);
// }
