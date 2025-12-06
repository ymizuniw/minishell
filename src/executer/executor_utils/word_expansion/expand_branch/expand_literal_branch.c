/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_literal_branch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:46:49 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:48:48 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
