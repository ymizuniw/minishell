#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_expand_value(void)
{
	t_token	*token;
	char	*result;

	token = alloc_token();
	if (token)
	{
		bzero(token, sizeof(t_token));
		token->value = strdup("$HOME");
		result = expand_value(token);
		printf("expand_value test completed\n");
		if (result)
		{
			printf("Expanded value: %s\n", result);
			free(result);
		}
		free_token_list(token);
	}
}

int	main(void)
{
	test_expand_value();
	return (0);
}
