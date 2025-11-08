
#include "minishell_libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*p;
	const unsigned char	*q;

	if (!dest && !src)
		return (NULL);
	if (n == 0)
		return (dest);
	i = 0;
	p = (unsigned char *)dest;
	q = (const unsigned char *)src;
	while (i < n)
	{
		p[i] = q[i];
		i++;
	}
	return (dest);
}
