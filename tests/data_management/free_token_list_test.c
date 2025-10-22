#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_free_token_list(void)
{
	t_token	*token;

	token = alloc_token();
	if (token)
	{
		bzero(token, sizeof(t_token));
		// Add some test data if needed
		free_token_list(token);
		printf("free_token_list test passed\n");
	}
}

int	main(void)
{
	test_free_token_list();
	return (0);
}
