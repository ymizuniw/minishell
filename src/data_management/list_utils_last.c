/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_last.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 16:45:05 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stddef.h>

// Get the last token in the list
t_token	*token_last(t_token *head)
{
	return ((t_token *)ft_lst_last(head, offsetof(t_token, next)));
}

// Get the last redirection in the list
t_redir	*redir_last(t_redir *head)
{
	return ((t_redir *)ft_lst_last(head, offsetof(t_redir, next)));
}

// Get the last word in the list
t_word	*word_last(t_word *head)
{
	return ((t_word *)ft_lst_last(head, offsetof(t_word, next)));
}
