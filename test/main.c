/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 19:05:33 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 19:05:35 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	join_value(char **d, const char *s, size_t dlen, size_t slen)
{
	char	*new;

	new = malloc(sizeof(char) * (slen + dlen + 1));
	if (new == NULL)
		return (0);
	memset(new, 0, sizeof(char) * (slen + dlen + 1));
	if (*d)
		memcpy(new, *d, dlen);
	memcpy(new + dlen, s, slen);
	if (*d)
		free(*d);
	*d = new;
	return (1);
}

static bool	append_res(char **res, char c)
{
	char	tmp[2];
	size_t	res_len;

	if (*res != NULL)
		printf("append_res:%s\n", *res);
	tmp[0] = c;
	tmp[1] = '\0';
	res_len = 0;
	if (*res)
		res_len = strlen(*res);
	if (!join_value(res, tmp, res_len, 1))
		return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	char	*document;
	char	c;

	document = NULL;
	while (1)
	{
		scanf("%c", &c);
		if (c == '\n')
			continue ;
		if (c == 'E')
		{
			printf("document: %s\n", document);
			return (1);
		}
		append_res(&document, c);
		printf("document: %s", document);
	}
	return (0);
}
