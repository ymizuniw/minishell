/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:44 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 18:39:44 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void	*xmalloc(size_t size)
{
	void	*p;

	p = (void *)malloc(size);
	if (p == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	return (p);
}

void	*xcalloc(size_t size)
{
	void	*p;

	p = xmalloc(size);
	if (p != NULL)
		ft_memset(p, 0, size);
	return (p);
}
