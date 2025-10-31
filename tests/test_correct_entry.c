#include "includes/minishell.h"
#include <stdio.h>

void	fgen_tree(t_ast **parent, t_token **cur_token);

void	print_ast_simple(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	for (i = 0; i < depth; i++)
		printf("  ");
	printf("%s\n",
		node->type == NODE_CMD ? "CMD" : node->type == NODE_PIPE ? "PIPE" : node->type == NODE_AND ? "AND" : node->type == NODE_OR ? "OR" : "?");
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("L:\n");
		print_ast_simple(node->left, depth + 1);
	}
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("R:\n");
		print_ast_simple(node->right, depth + 1);
	}
}

int	main(void)
{
	t_ast	*root;

	t_token *head, *cur;
	root = NULL;
	printf("\n========================================\n");
	printf("CORRECT ENTRY POINT TEST\n");
	printf("========================================\n\n");
	// Test simple command first
	printf("Test 1: 'echo hello'\n");
	head = lexer("echo hello");
	printf("  Token chain: ");
	cur = head;
	while (cur)
	{
		printf("%s",
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->value ? cur->value : "?");
		if (cur->next)
			printf(" → ");
		cur = cur->next;
	}
	printf("\n");
	// CORRECT: Start from HEAD->next (leftmost)
	cur = head->next;
	if (cur && cur->type != TK_EOF)
	{
		printf("  Starting from: %s (HEAD->next)\n",
			cur->value ? cur->value : "?");
		root = NULL;
		fgen_tree(&root, &cur);
		printf("  Result: ");
		if (cur == head)
			printf("✓ Consumed to HEAD\n");
		else
			printf("✗ NOT at HEAD (type=%d)\n", cur->type);
		if (root)
		{
			printf("  AST:\n");
			print_ast_simple(root, 2);
		}
	}
	// Now test with pipe - this will likely still crash
	// because the design assumes right-to-left
	printf("\nTest 2: 'echo hello | cat'\n");
	head = lexer("echo hello | cat");
	printf("  Token chain: ");
	cur = head;
	while (cur)
	{
		printf("%s",
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_PIPE ? "PIPE" : cur->value ? cur->value : "?");
		if (cur->next)
			printf(" → ");
		cur = cur->next;
	}
	printf("\n");
	printf("  Starting from: HEAD->next (leftmost)\n");
	cur = head->next;
	if (cur && cur->type != TK_EOF)
	{
		root = NULL;
		printf("  First token: type=%d (%s)\n", cur->type,
			cur->type == TK_WORD ? "WORD" : cur->type == TK_PIPE ? "PIPE" : "?");
		// This will work if the parser handles it correctly
		fgen_tree(&root, &cur);
		printf("  Result: ");
		if (cur == head)
			printf("✓ Consumed to HEAD\n");
		else
			printf("✗ NOT at HEAD (type=%d)\n", cur->type);
		if (root)
		{
			printf("  AST:\n");
			print_ast_simple(root, 2);
		}
	}
	printf("\n========================================\n");
	printf("CONCLUSION:\n");
	printf("  Entry point should be HEAD->next\n");
	printf("  But the parser recursion logic may need adjustment\n");
	printf("  to handle operators correctly.\n");
	printf("========================================\n");
	return (0);
}
