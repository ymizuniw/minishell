/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_string.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 21:35:23 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int			expand_single_dollar(char **word_ptr, char **expanded, size_t *len,
				t_shell *shell);
int			append_to_expanded(char **expanded, size_t *len, const char *str,
				size_t str_len);

// Expand string with dollar variables (for double-quoted strings)
char	*expand_string_with_dollars(char *str, t_shell *shell)
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

static char	*expand_word_node(t_word *w, t_shell *shell)
{
	if (w->to_expand_doller && w->type == WD_DOLLER)
	{
		if (ft_strchr(w->word, '$'))
			return (expand_string_with_dollars(w->word, shell));
		else
			return (expand_doller(w, shell));
	}
	return (ft_strdup(w->word));
}

char	*expand_token_words(t_word *word_list, t_shell *shell)
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
		expanded = expand_word_node(w, shell);
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
