#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_free_subshell_tree(void)
{
	t_ast	*tree;

	tree = alloc_node();
	if (tree)
	{
		bzero(tree, sizeof(t_ast));
		// Add some test data if needed
		free_subshell_tree(tree);
		printf("free_subshell_tree test passed\n");
	}
}

int	main(void)
{
	test_free_subshell_tree();
	return (0);
}
