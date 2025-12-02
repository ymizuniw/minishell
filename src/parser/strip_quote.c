/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:35:55 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 20:36:29 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	strip_loop(char *res, const char *str, size_t len, bool *quoted)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (*quoted == false)
				*quoted = true;
			i++;
			continue ;
		}
		res[j] = str[i];
		i++;
		j++;
	}
	res[j] = '\0';
}

char	*strip_quote(char *str, bool *quoted)
{
	size_t	len;
	char	*res;

	len = 0;
	res = NULL;
	len = ft_strlen(str);
	res = malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		return (NULL);
	ft_bzero(res, sizeof(char) * (len + 1));
	strip_loop(res, str, len, quoted);
	return (res);
}
