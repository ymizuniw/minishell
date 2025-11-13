/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 17:26:17 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/13 12:27:01 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*tmp;
	const char	*ptr;

	ptr = s;
	tmp = NULL;
	while (*ptr)
	{
		if (*ptr == (char)c)
			tmp = ptr;
		ptr++;
	}
	if ((char)c == '\0')
		return ((char *)ptr);
	else if (tmp)
		return ((char *)tmp);
	else
		return (NULL);
}
