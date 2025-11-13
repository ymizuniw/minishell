/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:18:03 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/02 00:09:38 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n--)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>
// #include <stddef.h>

// void	*ft_memchr(const void *s, int c, size_t n);

// int	main(void)
// {
// 	const char	data[] = "Hello, 42Tokyo!";
// 	int			ch = '4';
// 	size_t		len = 15;

// 	void *result = ft_memchr(data, ch, len);
// 	if (result != NULL)
// 		printf("Found '%c' at offset %ld:
// \"%s\"\n", ch, (char *)result - data, (char *)result);
// 	else
// 		printf("Character '%c' not found in first %zu bytes.\n", ch, len);

// 	// 比較用: 標準関数との一致もチェック（任意）
// 	void *std_result = memchr(data, ch, len);
// 	if (result == std_result)
// 		printf("✅ Result matches standard memchr.\n");
// 	else
// 		printf("❌ Result does NOT match standard memchr.\n");

// 	return (0);
// }

// #include <stdio.h>
// int	main(void)
// {
// 	const char str[] = "abcdefg";
// 	size_t	n = 5;
// 	int		c = 98;
// 	char *res = ft_memchr(str, c, n);

// 	printf("%s\n", res);
// 	return (0);
// }
// void	*ft_memchr(const void *s, int c, size_t n)

// {

// 	const unsigned char	*str;

// 	size_t				i;

// 	str = (const unsigned char *)s;

// 	i = 0;

// 	while (i < n)

// 	{

// 		if (str[i] == (unsigned char)c)

// 			return ((void *)(str + i));

// 		i++;

// 	}

// 	return (NULL);

// }
