/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:29:22 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/05 21:54:28 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

static int	free_all(char **arr, int i)
{
	while (--i >= 0)
		free(arr[i]);
	free(arr);
	return (0);
}

static int	word_len(char const *s, char c)
{
	int	len_res;

	len_res = 0;
	while (*s && *s != c)
	{
		len_res++;
		s++;
	}
	return (len_res);
}

char	**ft_split(char const *s, char c)
{
	int		len;
	int		i;
	char	**res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			len = word_len(s, c);
			res[i] = ft_substr(s, 0, len);
			if (!res[i++])
				return (free_all(res, i), NULL);
			s += len;
		}
	}
	res[i] = NULL;
	return (res);
}
