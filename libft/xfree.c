#include "libft.h"

void	xfree(void *p)
{
	if (p != NULL)
		free(p);
}
