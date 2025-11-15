/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:35 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 20:08:15 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Check if character is a quote and return the quote type
unsigned char	is_quote(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	if (uc == '\"')
		return ('\"');
	else if (uc == '\'')
		return ('\'');
	else
	{
		return ('\0');
	}
}
