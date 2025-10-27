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
	memset(p, 0, size);
	return (0);
}
