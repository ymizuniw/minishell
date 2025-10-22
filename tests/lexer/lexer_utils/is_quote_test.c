#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_is_quote(void)
{
	unsigned char	result;

	result = is_quote('"');
	assert(result == '"');
	printf("is_quote test for '\"': passed\n");
	result = is_quote('\'');
	assert(result == '\'');
	printf("is_quote test for \"'\": passed\n");
	result = is_quote('a');
	assert(result == '\0');
	printf("is_quote test for 'a': passed\n");
	printf("is_quote tests completed\n");
}

int	main(void)
{
	test_is_quote();
	return (0);
}
