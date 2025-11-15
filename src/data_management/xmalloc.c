/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:51 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:36:53 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	*xmalloc(size_t size)
{
	void	*p;

	p = (void *)malloc(size);
	if (p == NULL)
		perror("malloc: ");
	return (p);
}

void	*xcalloc(size_t size)
{
	void	*p;

	p = xmalloc(size);
	ft_memset(p, 0, size);
	return (p);
}
