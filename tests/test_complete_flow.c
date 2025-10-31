#include "includes/minishell.h"
#include <assert.h>
#include <stdio.h>

// Forward declaration
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

void	test_case(const char *input, const char *description)
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
		printf("\n=== AST ===\n");
		print_ast(root, 0);
		printf("✓ PASS: parse() succeeded\n");
	}
	else
	{
		printf("✗ FAIL: parse() returned NULL\n");
	}
}

int	main(void)
{
	printf("================================================\n");
	printf("  COMPLETE FLOW TEST: Lexer + Parser\n");
	printf("================================================\n");
	test_case("echo hello", "Simple command");
	test_case("echo hello world", "Command with multiple args");
	test_case("ls -la /tmp", "Command with flags and path");
	test_case("cat file.txt", "Command with file");
	test_case("echo hello | cat", "Simple pipe");
	test_case("ls -l | grep txt", "Pipe with flags");
	test_case("cat file | grep error | wc -l", "Multiple pipes");
	test_case("echo a && echo b", "AND operator");
	test_case("echo a || echo b", "OR operator");
	test_case("ls && cat file || echo fail", "Mixed operators");
	printf("\n================================================\n");
	printf("  ALL TESTS COMPLETED\n");
	printf("================================================\n");
	return (0);
}
