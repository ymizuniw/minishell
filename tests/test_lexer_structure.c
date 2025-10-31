#include "includes/minishell.h"
#include <stdio.h>

void	print_token_chain(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("\n=== Token Chain (following ->next) ===\n");
	while (cur)
	{
		printf("[%d] Token %p: type=%d (%s) value='%s' prev=%p next=%p\n", i++,
			(void *)cur, cur->type,
			cur->type == TK_HEAD ? "TK_HEAD" : cur->type == TK_EOF ? "TK_EOF" : cur->type == TK_WORD ? "TK_WORD" : cur->type == TK_NEWLINE ? "TK_NEWLINE" : cur->type == TK_DOLLER ? "TK_DOLLER" : "OTHER",
			cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur->next);
		cur = cur->next;
		if (i > 20) // safety limit
		{
			printf("... (stopping after 20 tokens)\n");
			break ;
		}
	}
	printf("\n=== Token Chain (following ->prev from EOF) ===\n");
	// Find EOF
	cur = head;
	while (cur && cur->next)
		cur = cur->next;
	i = 0;
	while (cur)
	{
		printf("[%d] Token %p: type=%d (%s) value='%s'\n", i++, (void *)cur,
			cur->type,
			cur->type == TK_HEAD ? "TK_HEAD" : cur->type == TK_EOF ? "TK_EOF" : cur->type == TK_WORD ? "TK_WORD" : cur->type == TK_NEWLINE ? "TK_NEWLINE" : cur->type == TK_DOLLER ? "TK_DOLLER" : "OTHER",
			cur->value ? cur->value : "(null)");
		cur = cur->prev;
		if (i > 20)
		{
			printf("... (stopping after 20 tokens)\n");
			break ;
		}
	}
}

int	main(void)
{
	t_token	*head;

	printf("Test 1: Simple command 'echo hello'\n");
	head = lexer("echo hello");
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return (1);
	}
	print_token_chain(head);
	printf("\n\nTest 2: Empty input ''\n");
	head = lexer("");
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return (1);
	}
	print_token_chain(head);
	return (0);
}
