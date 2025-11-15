/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_quote_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:40:02 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:40:03 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Set either double or single quotation flag to token
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
