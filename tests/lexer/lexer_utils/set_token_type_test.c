#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_set_token_type(void)
{
	t_token	*token;
	char	*input;
	size_t	tmp_idx;

	token = alloc_token();
	input = "echo hello";
	tmp_idx = 0;
	if (token)
	{
		bzero(token, sizeof(t_token));
		set_token_type(token, input, &tmp_idx);
		printf("set_token_type test completed\n");
		free(token);
	}
}

int	main(void)
{
	test_set_token_type();
	return (0);
}
