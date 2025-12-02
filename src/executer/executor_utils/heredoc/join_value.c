/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_value.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:52:45 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:56:40 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	join_value(char **d, const char *s, size_t dlen, size_t slen)
{
	char	*new;

	new = malloc(sizeof(char) * (slen + dlen + 1));
	if (new == NULL)
		return (0);
	ft_memset(new, 0, sizeof(char) * (slen + dlen + 1));
	if (*d)
		ft_memcpy(new, *d, dlen);
	ft_memcpy(new + dlen, s, slen);
	if (*d)
		free(*d);
	*d = new;
	return (1);
}
