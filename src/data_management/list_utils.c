/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/11 10:53:50 by ymizuniw         ###   ########.fr       */
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

// Add redirection to the end of the list
void	redir_add_back(t_redir **head, t_redir *new)
{
	t_redir	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = redir_last(*head);
	last->next = new;
}

// Add word to the end of the list
void	word_add_back(t_word **head, t_word *new)
{
	t_word	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = word_last(*head);
	last->next = new;
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
