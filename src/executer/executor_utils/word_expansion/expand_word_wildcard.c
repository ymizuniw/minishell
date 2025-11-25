/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_wildcard.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/26 01:04:18 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char    *expand_doller_word(const char *var, t_shell *shell)
{
    char    *env_value;

    if (!var || !shell)
        return (ft_strdup(""));
    if (ft_strcmp(var, "?") == 0)
        return ft_itoa(shell->last_exit_status);
    env_value = get_env_value(shell->env_list, var);
    if (!env_value)
        return (ft_strdup(""));
    return ft_strdup(env_value);
}

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

static char	**create_fallback_result(t_word *word_list, t_shell *shell)
{
	char	**result;

	result = (char **)xcalloc(sizeof(char *) * 2);
	if (!result)
		return (NULL);
	result[0] = expand_plain_word(word_list, shell);
	if (!result[0])
		return (free_double_array(result), NULL);
	result[1] = NULL;
	return (result);
}

char *get_word_string(t_word *word)
{
	if (!word)
		return (ft_strdup(""));
	if (word->type!=WD_OTHER)
		return (ft_strdup(word->word));
	return (NULL);
}

char *expand_plain_word(t_word *list, t_shell *shell)
{
	char *result;
	char *tmp;

	if (!list)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while(list)
	{
		if (list->type==WD_DOLLER)
		{
			char *expanded = expand_doller_word(list->word, shell);
			if (!expanded)
				return (xfree(result), NULL);
			tmp = ft_strjoin(result, expanded);
			xfree(expanded);
			xfree(result);
			result = tmp;
		}
		else
		{
			char *literal = get_word_string(list);
			if (!literal)
				return (xfree(result), NULL);
			tmp = ft_strjoin(result, literal);
			xfree(literal);
			xfree(result);
			result = tmp;		
		}
		list = list->next;
	}
	return (result);
}

char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
		size_t *result_count)
{
	char	*pattern;
	char	**wildcard_results;
	size_t	count;

	(void)shell;
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
