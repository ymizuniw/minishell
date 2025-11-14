/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_unclosed_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/14 00:00:00 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Check if input has unclosed quotes
** Returns:
**   0 if all quotes are properly closed
**   1 if there's an unclosed quote (syntax error)
*/
int	check_unclosed_quotes(const char *input)
{
	size_t	i;
	char	quote;

	i = 0;
	while (input[i])
	{
		quote = is_quote(input[i]);
		if (quote != '\0')
		{
			i++;
			while (input[i] && input[i] != quote)
				i++;
			if (input[i] != quote)
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}
