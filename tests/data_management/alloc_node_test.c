#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_alloc_node(void)
{
	t_ast	*node;

	node = alloc_node();
	assert(node != NULL);
	printf("alloc_node test passed\n");
	free(node);
}

int	main(void)
{
	test_alloc_node();
	return (0);
}
