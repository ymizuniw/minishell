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
		memset(p, 0, size);
	return (p);
}
