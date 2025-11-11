#include "../includes/minishell.h"
#include <stdio.h>

// Stub for missing function
void	free_hist_box(char *hist_box[HIST_MAX])
{
	(void)hist_box;
}

void	print_tokens(t_token *tokens)
{
	t_token	*cur;
	int		i;

	printf("--- Forward traversal (head->next->...) ---\n");
	cur = tokens->next;
	i = 0;
	while (cur)
	{
		printf("Token %d: type=%d value='%s' prev=%p cur=%p next=%p\n", i,
			cur->type, cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur, (void *)cur->next);
		cur = cur->next;
		i++;
	}
	printf("\n--- Backward traversal (tail->prev->...) ---\n");
	// Find tail (EOF token)
	cur = tokens->next;
	while (cur && cur->next)
		cur = cur->next;
	// Traverse backwards
	i = 0;
	while (cur && cur != tokens)
	{
		printf("Token %d (backward): type=%d value='%s'\n", i, cur->type,
			cur->value ? cur->value : "(null)");
		cur = cur->prev;
		i++;
	}
}

int	main(void)
{
	t_token	*tokens;

	printf("=== Test 1: Simple echo ===\n");
	tokens = lexer("echo hello");
	if (tokens)
	{
		print_tokens(tokens);
		free_token_list(tokens);
	}
	printf("\n=== Test 2: Echo with arguments ===\n");
	tokens = lexer("echo hello world");
	if (tokens)
	{
		print_tokens(tokens);
		free_token_list(tokens);
	}
	printf("\n=== Test 3: Command with pipe ===\n");
	tokens = lexer("echo test | cat");
	if (tokens)
	{
		print_tokens(tokens);
		free_token_list(tokens);
	}
	return (0);
}
