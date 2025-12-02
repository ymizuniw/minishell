/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_quote_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:40:02 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 19:56:34 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	set_quote_flag(t_token *token, char *input, char const quote_open)
{
	(void)input;
	if (!token)
		return ;
	if (quote_open == '\"')
		token->in_dquote = true;
	else if (quote_open == '\'')
		token->in_squote = true;
}
