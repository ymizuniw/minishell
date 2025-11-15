/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parenthesis_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 11:42:42 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_empty_parenthesis(t_token *lparen)
{
	t_token	*next;

	if (!lparen || lparen->type != TK_LPAREN)
		return (1);
	next = lparen->next;
	if (next && next->type == TK_RPAREN)
		return (0);
	return (1);
}

int	process_lparen(t_token *cur)
{
	if (!check_empty_parenthesis(cur))
		return (-1);
	return (1);
}

int	process_rparen(t_token *cur)
{
	int	check;
	int	check_parenthesis(t_token *token);

	check = check_parenthesis(cur);
	if (check < 0)
		return (-1);
	return (0);
}

int	check_backward_balance(t_token *cur, int *balance)
{
	if (cur->type == TK_LPAREN)
	{
		if (cur->prev && cur->prev->type == TK_RPAREN)
			return (-1);
		(*balance)++;
	}
	else if (cur->type == TK_RPAREN)
	{
		(*balance)--;
		if (*balance < 0)
			return (-1);
	}
	return (0);
}
