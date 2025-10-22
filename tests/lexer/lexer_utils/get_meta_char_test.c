#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_is_meta_char(void)
{
	t_metachar	result;

	result = is_meta_char("|");
	printf("is_meta_char test for '|': %d\n", result);
	result = is_meta_char("&");
	printf("is_meta_char test for '&': %d\n", result);
	result = is_meta_char("a");
	printf("is_meta_char test for 'a': %d\n", result);
	printf("is_meta_char tests completed\n");
}

int	main(void)
{
	test_is_meta_char();
	return (0);
}
