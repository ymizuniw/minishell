/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_plain_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:29:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:49:40 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static char	*init_expand_result(bool *quoted)
{
	*quoted = false;
	return (ft_strdup(""));
}

static int	handle_dollar(char **result, t_word *node, t_shell *shell)
{
	return (expand_word_branch(result, node, shell));
}

static int	handle_literal(char **result, t_word *node)
{
	return (expand_literal_branch(result, node));
}

static int	process_word_node(char **result, t_word *node, t_shell *shell,
		bool *quoted)
{
	if (node->quoted)
		*quoted = true;
	if (node->type == WD_DOLLER)
		return (handle_dollar(result, node, shell));
	else
		return (handle_literal(result, node));
}

char	*expand_plain_word(t_word *list, t_shell *shell, bool *quoted)
{
	char	*result;

	if (!list)
		return (ft_strdup(""));
	result = init_expand_result(quoted);
	if (!result)
		return (NULL);
	while (list)
	{
		if (process_word_node(&result, list, shell, quoted) < 0)
		{
			xfree(result);
			return (NULL);
		}
		list = list->next;
	}
	return (result);
}
