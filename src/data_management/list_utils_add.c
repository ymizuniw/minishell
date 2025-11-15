/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:42 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:36:43 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stddef.h>

// Add redirection to the end of the list
void	redir_add_back(t_redir **head, t_redir *new)
{
	ft_lst_add_back((void **)head, new, offsetof(t_redir, next));
}

// Add word to the end of the list
void	word_add_back(t_word **head, t_word *new)
{
	ft_lst_add_back((void **)head, new, offsetof(t_word, next));
}

// Add token to the end of the list
void	token_add_back(t_token **head, t_token *new)
{
	t_token	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = token_last(*head);
	last->next = new;
	new->prev = last;
}
