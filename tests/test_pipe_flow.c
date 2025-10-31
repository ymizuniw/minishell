#include "includes/minishell.h"
#include <stdio.h>

void	fgen_tree(t_ast **parent, t_token **cur_token);

void	debug_tokens(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("\nToken list:\n");
	while (cur && i < 20)
	{
		printf("  [%d] %p: type=%d (%s) value='%s' prev=%p next=%p\n", i++,
			(void *)cur, cur->type,
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_PIPE ? "PIPE" : cur->type == TK_WORD ? "WORD" : "?",
			cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur->next);
		cur = cur->next;
	}
}

int	main(void)
{
	t_ast	*root;

	t_token *head, *cur;
	root = NULL;
	printf("=== Testing PIPE flow ===\n");
	printf("Input: 'echo hello | cat'\n");
	head = lexer("echo hello | cat");
	if (!head)
	{
		printf("ERROR: lexer failed\n");
		return (1);
	}
	debug_tokens(head);
	printf("\n=== Manual step-by-step parsing ===\n");
	// Start from HEAD->next as per current design
	cur = head->next;
	printf("\n1. Starting token: %p type=%d (%s) value='%s'\n", (void *)cur,
		cur->type, cur->type == TK_WORD ? "WORD" : "?",
		cur->value ? cur->value : "(null)");
	printf("2. Calling fgen_tree with parent=NULL, cur='%s'\n",
		cur->value ? cur->value : "(null)");
	printf("\n3. In fgen_tree:\n");
	printf("   - cur->type = %d (%s)\n", cur->type,
		cur->type == TK_WORD ? "WORD" : "?");
	printf("   - token_is_command(cur->type) = %d\n", cur->type == TK_WORD
		|| cur->type == TK_DOLLER ? 1 : 0);
	printf("   - Will call gen_command_node\n");
	printf("\n4. In gen_command_node:\n");
	printf("   - Calls parse_command_list with cur='%s'\n",
		cur->value ? cur->value : "(null)");
	printf("\n5. In parse_command_list:\n");
	printf("   - Starting at: '%s'\n", cur->value ? cur->value : "(null)");
	printf("   - cur->next = %p type=%d (%s)\n", (void *)cur->next,
		cur->next ? cur->next->type : -1, cur->next
		&& cur->next->type == TK_PIPE ? "PIPE" : cur->next
		&& cur->next->type == TK_WORD ? "WORD" : "?");
	printf("\n   OLD LOGIC (backward):\n");
	printf("   - Move RIGHT via ->next to find rightmost command\n");
	printf("   - while (tmp->next && token_is_command(tmp->next->type))\n");
	printf("   - From 'cat': cat->next=PIPE (not command), stop\n");
	printf("   - Then process LEFT via ->prev\n");
	printf("   - Process 'cat', then cat->prev=HEAD (not command), stop\n");
	printf("   - Sets cur_token = HEAD (wrong for forward flow!)\n");
	printf("\n   FORWARD LOGIC (what we need):\n");
	printf("   - Process current and following commands\n");
	printf("   - From 'cat': process 'cat'\n");
	printf("   - Move to cat->next = PIPE (not command), stop\n");
	printf("   - Sets cur_token = PIPE\n");
	printf("\n6. Back in fgen_tree:\n");
	printf("   - Built CMD node for 'cat'\n");
	printf("   - cur_token now points to: PIPE\n");
	printf("   - Recurses: fgen_tree(&left, &PIPE)\n");
	printf("\n7. In fgen_tree again with PIPE:\n");
	printf("   - cur->type = PIPE\n");
	printf("   - token_is_operator(PIPE) = true\n");
	printf("   - Calls swap_with_parent(&left, &PIPE)\n");
	printf("   - But 'left' is NULL!\n");
	printf("\n8. In swap_with_parent:\n");
	printf("   - *parent = NULL (the 'left' pointer)\n");
	printf("   - node->right = *parent; // NULL, ok\n");
	printf("   - node->parent = (*parent)->parent;
		// CRASH! NULL dereference\n");
	printf("\n=== THE PROBLEM ===\n");
	printf("The old algorithm expects:\n");
	printf("  - Start from RIGHTMOST token\n");
	printf("  - Build right side first\n");
	printf("  - Then operator\n");
	printf("  - Then left side\n");
	printf("\n");
	printf("But with forward reading:\n");
	printf("  - Start from LEFTMOST token\n");
	printf("  - Build left side first ('cat')\n");
	printf("  - Then hit operator with no right side yet\n");
	printf("  - CRASH!\n");
	printf("\n=== THE SOLUTION ===\n");
	printf("For forward reading (HEAD to EOF):\n");
	printf("  1. Build left side\n");
	printf("  2. When hitting operator:\n");
	printf("     - Make operator the new parent\n");
	printf("     - Attach previous tree as LEFT child (not right!)\n");
	printf("     - Continue forward for right side\n");
	printf("  3. Build right side\n");
	printf("  4. Attach as right child\n");
	return (0);
}
