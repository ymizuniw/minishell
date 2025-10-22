#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_free_double_array(void)
{
	char	**array;

	array = malloc(sizeof(char *) * 3);
	array[0] = strdup("test1");
	array[1] = strdup("test2");
	array[2] = NULL;
	free_double_array(array);
	printf("free_double_array test passed\n");
}

int	main(void)
{
	test_free_double_array();
	return (0);
}
