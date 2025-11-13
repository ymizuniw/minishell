/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:26:17 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/02 17:12:25 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = 0;
		i++;
	}
}

// #include <stdio.h>
// int	main(void)
// {
// 	char str[] = "renkon";
// 	ft_bzero(str, 3);
// 	printf("str + 3 = %s\n", str + 3);
// 	return (0);
// }
