#include "../../includes/minishell.h"

void	xfree(void *p)
{
	if (p != NULL)
		free(p);
}
