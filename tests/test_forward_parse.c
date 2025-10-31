#include "includes/minishell.h"
#include <assert.h>
#include <stdio.h>

// Forward declaration
t_ast	*parse(t_token *token_head);

void	print_token_chain(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("\n=== Token Chain ===\n");
	while (cur)
	{
		printf("[%d] %p: type=%d (%s) value='%s' prev=%p next=%p\n", i++,
			(void *)cur, cur->type,
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_WORD ? "WORD" : cur->type == TK_PIPE ? "PIPE" : "OTHER",
			cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur->next);
		cur = cur->next;
		if (i > 20)
			break ;
	}
}

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

int	main(void)
{
	t_token	*head;
	t_ast	*root;

	printf("=== Test 1: Simple command ===\n");
	printf("Input: 'echo hello'\n");
	head = lexer("echo hello");
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return (1);
	}
	print_token_chain(head);
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
	printf("\n\n=== Test 2: Pipe command ===\n");
	printf("Input: 'echo hello | cat'\n");
	head = lexer("echo hello | cat");
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return (1);
	}
	print_token_chain(head);
	root = parse(head);
	if (root)
	{
		printf("\n=== AST ===\n");
		print_ast(root, 0);
		printf("✓ PASS: parse() succeeded\n");
		// Validate structure
		if (root->type == NODE_PIPE)
		{
			printf("✓ PASS: Root is PIPE node\n");
			if (root->left && root->left->type == NODE_CMD)
				printf("✓ PASS: Left child is CMD\n");
			else
				printf("✗ FAIL: Left child is not CMD\n");
			if (root->right && root->right->type == NODE_CMD)
				printf("✓ PASS: Right child is CMD\n");
			else
				printf("✗ FAIL: Right child is not CMD\n");
		}
		else
		{
			printf("✗ FAIL: Root is not PIPE node\n");
		}
	}
	else
	{
		printf("✗ FAIL: parse() returned NULL\n");
	}
	return (0);
}
