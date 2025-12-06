/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_branch.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:46:34 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:50:31 by ymizuniw         ###   ########.fr       */
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
