#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_syntax_checker(void)
{
	t_token	*token;
	int		result;

	token = alloc_token();
	if (token)
	{
		bzero(token, sizeof(t_token));
		token->value = strdup("echo hello");
		result = syntax_checker(token);
		printf("syntax_checker test result: %d\n", result);
		free_token_list(token);
	}
}

int	main(void)
{
	test_syntax_checker();
	return (0);
}
