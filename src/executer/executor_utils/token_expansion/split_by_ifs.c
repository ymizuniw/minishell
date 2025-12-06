/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_by_ifs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:51:59 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:52:12 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	ft_isifs(char s)
{
	return (s == ' ' || s == '\t' || s == '\n');
}

static size_t	count_word(char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break ;
		while (str[i] && !ft_isifs(str[i]))
			i++;
		j++;
	}
	return (j);
}

static int	extract_word(char **result, size_t *idx, char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] && !ft_isifs(str[*i]))
		(*i)++;
	result[*idx] = ft_substr(str, start, *i - start);
	if (!result[*idx])
	{
		while (*idx > 0)
		{
			(*idx)--;
			free(result[*idx]);
		}
		free(result);
		return (0);
	}
	(*idx)++;
	return (1);
}

char	**split_by_ifs(char *str)
{
	size_t	i;
	size_t	word;
	char	**result;
	size_t	idx;

	i = 0;
	word = count_word(str);
	result = xmalloc(sizeof(char *) * (word + 1));
	if (!result)
		return (NULL);
	idx = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break ;
		if (!extract_word(result, &idx, str, &i))
			return (NULL);
	}
	result[idx] = NULL;
	return (result);
}
