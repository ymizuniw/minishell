/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepend_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:38 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 18:39:40 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Prepend new token at head (LIFO)
void	prepend_tokens(t_token *head, t_token *new)
{
	t_token	*cur_first;

	cur_first = NULL;
	if (!head || !new)
		return ;
	if (head->next)
		cur_first = head->next;
	if (cur_first != NULL)
		cur_first->prev = new;
	head->next = new;
	new->next = cur_first;
	new->prev = head;
	head->size++;
}
