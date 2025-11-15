/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 17:30:54 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*ext_unit(char *src, size_t start, size_t end)
{
	return (ft_substr(src, start, end - start));
}

int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = ft_realloc(*res, sizeof(char) * (size1 + 1), sizeof(char) * (size1
				+ size2 + 1));
	if (*res == NULL)
		return (0);
	ft_memcpy(*res + size1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
}

size_t	quote_close_place(char quote, char *value, size_t i)
{
	if (value == NULL)
		return (0);
	while (value[i] != '\0')
	{
		if (value[i] == quote)
			return (i);
		i++;
	}
	return (0);
}

int	init_gen_word_data(t_word **word, t_gen_word *gw, char *value, size_t *i)
{
	*word = (t_word *)xcalloc(sizeof(t_word));
	if (*word == NULL)
		return (-1);
	ft_memset(gw, 0, sizeof(t_gen_word));
	gw->quote = is_quote(value[*i]);
	gw->close_place = 0;
	gw->word = *word;
	return (1);
}
