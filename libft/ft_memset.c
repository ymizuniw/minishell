
#include "minishell_libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = (unsigned char *)s;
	while (n > i)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

