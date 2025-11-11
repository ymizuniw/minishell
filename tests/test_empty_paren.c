#include "../includes/minishell.h"
#include <stdio.h>

void	free_hist_box(char *hist_box[HIST_MAX])
{
	(void)hist_box;
}

int	main(void)
{
	t_token	*tokens;
	t_token	*cur;
	int		i;
	int		result;

	printf("=== Test: () ===\n");
	tokens = lexer("()");
	if (!tokens)
	{
		printf("Lexer failed\n");
		return (1);
	}
	printf("Token list (forward from head):\n");
	cur = tokens->next;
	i = 0;
	while (cur)
	{
		printf("  [%d] type=%d value='%s' prev=%p cur=%p next=%p\n", i,
			cur->type, cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur, (void *)cur->next);
		cur = cur->next;
		i++;
	}
	printf("\nChecking parenthesis balance...\n");
	result = check_parenthesis_balance(tokens);
	printf("Result: %d (0=balanced, -1=error, 1=unclosed)\n", result);
	free_token_list(tokens);
	return (0);
}
