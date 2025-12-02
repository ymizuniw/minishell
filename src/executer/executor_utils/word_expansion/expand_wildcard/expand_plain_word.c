/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_plain_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:29:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 18:47:37 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	expand_word_branch(char **result, t_word *list, t_shell *shell)
{
	char	*tmp;
	char	*expanded;

	expanded = expand_doller_word(list->word, shell);
	if (!expanded)
	{
		xfree(*result);
		*result = NULL;
		return (-1);
	}
	tmp = ft_strjoin(*result, expanded);
	xfree(expanded);
	xfree(*result);
	*result = tmp;
	if (tmp == NULL)
		return (-1);
	return (1);
}

int	expand_literal_branch(char **result, t_word *list)
{
	char	*literal;
	char	*tmp;

	literal = get_word_string(list);
	if (!literal)
	{
		xfree(*result);
		*result = NULL;
		return (-1);
	}
	tmp = ft_strjoin(*result, literal);
	xfree(literal);
	xfree(*result);
	*result = tmp;
	if (*result == NULL)
		return (-1);
	return (1);
}

char	*expand_plain_word(t_word *list, t_shell *shell)
{
	char	*result;

	if (!list)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (list)
	{
		if (list->type == WD_DOLLER)
		{
			if (expand_word_branch(&result, list, shell) < 0)
				return (NULL);
		}
		else
		{
			if (expand_literal_branch(&result, list) < 0)
				return (NULL);
		}
		list = list->next;
	}
	return (result);
}
