/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 19:34:06 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/02 02:51:02 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char	*ext_unit(char *s, size_t start, size_t end)
{
	return (ft_strndup(&s[start], end - start));
}

int	join_value(char **d, const char *s, size_t dlen, size_t slen)
{
	char	*new;

	new = xcalloc(sizeof(char) * (slen + dlen + 1));
	if (new == NULL)
		return (0);
	ft_memcpy(new, *d, dlen);
	ft_memcpy(new + dlen, s, slen);
	return (1);
}
