#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_alloc_cmd(void)
{
	t_cmd	*cmd;

	cmd = alloc_cmd();
	assert(cmd != NULL);
	printf("alloc_cmd test passed\n");
	free(cmd);
}

int	main(void)
{
	test_alloc_cmd();
	return (0);
}
