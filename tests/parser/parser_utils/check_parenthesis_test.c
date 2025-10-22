#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_check_parenthesis(void)
{
	t_token	*token;
	int		result;

	token = alloc_token();
	if (token)
	{
		bzero(token, sizeof(t_token));
		token->value = strdup("(echo hello)");
		result = check_parenthesis(token);
		printf("check_parenthesis test result: %d\n", result);
		free_token_list(token);
	}
}

int	main(void)
{
	test_check_parenthesis();
	return (0);
}
