#include "includes/minishell.h"
#include <assert.h>
#include <stdio.h>

// Forward declarations from fucking_gen_tree.c
void	fgen_tree(t_ast **parent, t_token **cur_token);

void	print_token_chain(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("\n=== Token Chain ===\n");
	while (cur)
	{
		printf("[%d] %p: type=%d (%s) value='%s' next=%p\n", i++, (void *)cur,
			cur->type,
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_WORD ? "WORD" : "OTHER",
			cur->value ? cur->value : "(null)", (void *)cur->next);
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
				printf(",");
		}
		printf("]");
	}
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
	t_token	*cur;
	t_ast	*root;

	root = NULL;
	printf("=== Test: Lexer + Parser Integration ===\n");
	printf("Input: 'echo hello'\n");
	head = lexer("echo hello");
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return (1);
	}
	print_token_chain(head);
	// Find the rightmost command token (last WORD before EOF)
	// Chain is: HEAD -> ... -> EOF
	// We need to start from the last command token before EOF
	cur = head->next; // Skip HEAD
	if (!cur || cur->type == TK_EOF)
	{
		printf("ERROR: No tokens after HEAD\n");
		return (1);
	}
	// Find the last command token (rightmost before EOF)
	while (cur->next && cur->next->type != TK_EOF && (cur->next->type == TK_WORD
			|| cur->next->type == TK_DOLLER))
	{
		cur = cur->next;
	}
	printf("\nStarting parse from token: %p type=%d value='%s'\n", (void *)cur,
		cur->type, cur->value ? cur->value : "(null)");
	fgen_tree(&root, &cur);
	printf("\nAfter fgen_tree:\n");
	printf("  cur=%p type=%d (%s)\n", (void *)cur, cur->type,
		cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_WORD ? "WORD" : "OTHER");
	printf("  root=%p\n", (void *)root);
	if (root)
	{
		printf("\n=== AST ===\n");
		print_ast(root, 0);
	}
	// Expected: cur should be at HEAD
	printf("\n=== Validation ===\n");
	if (cur == head)
		printf("✓ PASS: cur_token consumed to HEAD\n");
	else
		printf("✗ FAIL: cur_token NOT at HEAD (cur=%p head=%p)\n", (void *)cur,
			(void *)head);
	if (root && root->type == NODE_CMD)
		printf("✓ PASS: Root is CMD node\n");
	else
		printf("✗ FAIL: Root is not CMD node\n");
	return (0);
}
