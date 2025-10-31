#include "includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	fgen_tree(t_ast **parent, t_token **cur_token);

void	test_case(const char *input, const char *description)
{
	t_ast	*root;
	int		hops;

	t_token *head, *cur, *start;
	root = NULL;
	printf("\n=== Test: %s ===\n", description);
	printf("Input: '%s'\n", input);
	head = lexer(input);
	if (!head)
	{
		printf("✗ FAIL: lexer returned NULL\n");
		return ;
	}
	// Verify HEAD token
	if (head->type != TK_HEAD)
	{
		printf("✗ FAIL: First token is not TK_HEAD (type=%d)\n", head->type);
		return ;
	}
	printf("✓ HEAD token created correctly (type=%d)\n", head->type);
	// Find EOF
	cur = head;
	while (cur->next)
		cur = cur->next;
	if (cur->type != TK_EOF)
	{
		printf("✗ FAIL: Last token is not TK_EOF (type=%d)\n", cur->type);
		return ;
	}
	printf("✓ EOF token created correctly (type=%d)\n", cur->type);
	// Verify prev pointer from EOF reaches HEAD
	cur = cur->prev;
	hops = 0;
	while (cur && cur->type != TK_HEAD && hops < 100)
	{
		cur = cur->prev;
		hops++;
	}
	if (!cur || cur->type != TK_HEAD)
	{
		printf("✗ FAIL: Cannot reach HEAD from EOF via prev pointers\n");
		return ;
	}
	printf("✓ Can traverse from EOF back to HEAD (%d hops)\n", hops);
	// Find rightmost command token for parsing
	start = head->next;
	if (!start || start->type == TK_EOF)
	{
		printf("  (No command tokens - empty input)\n");
		return ;
	}
	// Move to rightmost command token
	while (start->next && start->next->type != TK_EOF
		&& (start->next->type == TK_WORD || start->next->type == TK_DOLLER))
	{
		start = start->next;
	}
	printf("  Starting parse from: type=%d value='%s'\n", start->type,
		start->value ? start->value : "(null)");
	cur = start;
	fgen_tree(&root, &cur);
	// Verify cur_token moved to HEAD
	if (cur != head)
	{
		printf("✗ FAIL: cur_token not at HEAD after parse (cur->type=%d)\n",
			cur->type);
		return ;
	}
	printf("✓ cur_token correctly consumed to HEAD\n");
	if (root)
		printf("✓ AST root created (type=%d)\n", root->type);
	else
		printf("  (No AST root - expected for empty input)\n");
	printf("✓ PASS: %s\n", description);
}

int	main(void)
{
	printf("========================================\n");
	printf("  TK_HEAD and TK_EOF Handling Tests\n");
	printf("========================================\n");
	test_case("echo hello", "Simple command");
	test_case("ls -la", "Command with argument");
	test_case("cat", "Single word command");
	test_case("echo hello world foo bar", "Multiple arguments");
	test_case("", "Empty input");
	printf("\n========================================\n");
	printf("  All tests completed\n");
	printf("========================================\n");
	return (0);
}
