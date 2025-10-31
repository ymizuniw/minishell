#include "includes/minishell.h"
#include <stdio.h>

void	fgen_tree(t_ast **parent, t_token **cur_token);

void	test_with_operators(const char *input)
{
	t_ast	*root;
	int		count;

	t_token *head, *cur;
	root = NULL;
	printf("\n========================================\n");
	printf("Input: '%s'\n", input);
	printf("========================================\n");
	head = lexer(input);
	if (!head)
	{
		printf("ERROR: lexer returned NULL\n");
		return ;
	}
	// Show token chain
	printf("\nToken chain:\n");
	cur = head;
	count = 0;
	while (cur && count < 20)
	{
		printf("  [%d] type=%d (%s) value='%s'\n", count++, cur->type,
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_WORD ? "WORD" : cur->type == TK_PIPE ? "PIPE" : cur->type == TK_REDIRECT_OUT ? "REDIR_OUT" : cur->type == TK_REDIRECT_IN ? "REDIR_IN" : cur->type == TK_AND_IF ? "AND_IF" : cur->type == TK_OR_IF ? "OR_IF" : "OTHER",
			cur->value ? cur->value : "(null)");
		cur = cur->next;
	}
	// Find last token before EOF
	cur = head;
	while (cur->next && cur->next->type != TK_EOF)
		cur = cur->next;
	printf("\nStarting parse from: type=%d (%s) value='%s'\n", cur->type,
		cur->type == TK_WORD ? "WORD" : cur->type == TK_PIPE ? "PIPE" : cur->type == TK_REDIRECT_OUT ? "REDIR_OUT" : "OTHER",
		cur->value ? cur->value : "(null)");
	root = NULL;
	fgen_tree(&root, &cur);
	printf("\nAfter parse:\n");
	printf("  cur_token: type=%d (%s)\n", cur->type,
		cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_PIPE ? "PIPE" : "OTHER");
	if (root)
	{
		printf("  root: type=%d (%s)\n", (int)root->type,
			root->type == NODE_CMD ? "CMD" : root->type == NODE_PIPE ? "PIPE" : root->type == NODE_AND ? "AND" : root->type == NODE_OR ? "OR" : "OTHER");
	}
	else
	{
		printf("  root: NULL\n");
	}
}

int	main(void)
{
	test_with_operators("echo hello");
	test_with_operators("cat < input.txt");
	test_with_operators("ls > output.txt");
	test_with_operators("echo hello | cat");
	test_with_operators("ls && echo done");
	test_with_operators("false || echo backup");
	printf("\n========================================\n");
	printf("ISSUE ANALYSIS\n");
	printf("========================================\n");
	printf("\nThe problem:\n");
	printf("For 'echo hello | cat', the lexer creates:\n");
	printf("  HEAD -> cat -> | -> hello -> echo -> EOF\n");
	printf("\nThe parser starts from 'echo' (last before EOF)\n");
	printf("But with operators, this might not be correct!\n");
	printf("\nFor pipes/operators, the ENTIRE line should be parsed,\n");
	printf("not just from the last token.\n");
	printf("\nThe parser should actually start from EOF->prev\n");
	printf("which could be ANY type of token.\n");
	return (0);
}
