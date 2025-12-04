/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_wildcard_to_expand.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:28:17 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:30:12 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
