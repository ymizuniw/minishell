#include "includes/minishell.h"
#include <assert.h>
#include <stdio.h>

t_ast	*parse(t_token *token_head);

void	print_ast(t_ast *node, int depth)
{
	int		i;
	t_argv	*arg;

	if (!node)
		return ;
	for (i = 0; i < depth; i++)
		printf("  ");
	printf("Node type=%d", node->type);
	if (node->type == NODE_CMD && node->cmd)
	{
		printf(" CMD argv=[");
		arg = (t_argv *)node->cmd->argv;
		while (arg)
		{
			printf("%s", arg->word);
			arg = arg->next;
			if (arg)
				printf(", ");
		}
		printf("]");
	}
	else if (node->type == NODE_PIPE)
		printf(" PIPE");
	else if (node->type == NODE_AND)
		printf(" AND");
	else if (node->type == NODE_OR)
		printf(" OR");
	printf("\n");
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("LEFT:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("RIGHT:\n");
		print_ast(node->right, depth + 1);
	}
}

void	test_case(const char *input, const char *description,
		int should_succeed)
{
	t_token	*head;
	t_ast	*root;

	printf("\n=== Test: %s ===\n", description);
	printf("Input: '%s'\n", input);
	head = lexer(input);
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return ;
	}
	root = parse(head);
	if (root)
	{
		if (should_succeed)
		{
			printf("\n=== AST ===\n");
			print_ast(root, 0);
			printf("✓ PASS: parse() succeeded as expected\n");
		}
		else
		{
			printf("✗ FAIL: parse() should have returned NULL but got tree\n");
		}
	}
	else
	{
		if (!should_succeed)
			printf("✓ PASS: parse() returned NULL as expected\n");
		else
			printf("✗ FAIL: parse() returned NULL unexpectedly\n");
	}
}

int	main(void)
{
	printf("================================================\n");
	printf("  EDGE CASE TESTS\n");
	printf("================================================\n");
	// Valid cases
	test_case("echo", "Single command word", 1);
	test_case("a", "Single letter command", 1);
	test_case("a | b", "Minimal pipe", 1);
	test_case("a && b", "Minimal AND", 1);
	test_case("a || b", "Minimal OR", 1);
	test_case("a | b | c | d | e", "Long pipe chain", 1);
	test_case("a && b && c", "Multiple ANDs", 1);
	test_case("a || b || c", "Multiple ORs", 1);
	test_case("cmd arg1 arg2 arg3 arg4 arg5", "Many arguments", 1);
	// Mixed operators (right associative due to forward parsing)
	test_case("a | b && c", "Pipe then AND", 1);
	test_case("a && b | c", "AND then pipe", 1);
	test_case("a || b && c | d", "Complex mix", 1);
	// Empty/minimal
	test_case("", "Empty input", 0);
	printf("\n================================================\n");
	printf("  EDGE CASE TESTS COMPLETED\n");
	printf("================================================\n");
	return (0);
}
