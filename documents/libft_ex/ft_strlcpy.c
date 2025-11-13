/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 20:34:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/03 15:50:57 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	count;

	if (size == 0)
		return (ft_strlen(src));
	count = 0;
	while (src[count] && (count < size - 1))
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
	return (ft_strlen(src));
}

// #include <stdio.h>
// int	main(void)
// {
// 	char		d[20] = "abcdefghijklmn";
// 	const char	*s = "123456789";
// 	size_t		size1;
// 	size_t		res;

// 	size1 = ft_strlen(s) + 1;
// 	res = ft_strlcpy(d, s, size1);
// 	printf("the length of src : %zu\n", res);
// 	printf("new string : %s", d);
// 	return (0);
// }
