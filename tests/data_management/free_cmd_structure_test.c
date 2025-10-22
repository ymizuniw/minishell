#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_free_cmd_structure(void)
{
	t_cmd	*cmd;

	cmd = alloc_cmd();
	if (cmd)
	{
		bzero(cmd, sizeof(t_cmd));
		// Add some test data if needed
		free_cmd_structure(cmd);
		printf("free_cmd_structure test passed\n");
	}
}

int	main(void)
{
	test_free_cmd_structure();
	return (0);
}
