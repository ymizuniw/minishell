#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_signal_initializer(void)
{
	int	g_set;

	g_set = 0;
	signal_initializer(&g_set);
	printf("signal_initializer test completed\n");
	printf("g_set value after initialization: %d\n", g_set);
}

int	main(void)
{
	test_signal_initializer();
	return (0);
}
