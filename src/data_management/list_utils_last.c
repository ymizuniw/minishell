/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_last.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/13 18:16:26 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Get the last token in the list
t_token	*token_last(t_token *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

// Get the last redirection in the list
t_redir	*redir_last(t_redir *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

// Get the last word in the list
t_word	*word_last(t_word *head)
{
	if (!head)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}
