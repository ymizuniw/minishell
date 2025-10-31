#include "includes/minishell.h"
#include <stdio.h>

void	fgen_tree(t_ast **parent, t_token **cur_token);

void	analyze_flow(const char *input)
{
	t_ast	*root;
	int		count;
	t_argv	*arg;

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
	printf("\nToken chain (following ->next):\n");
	cur = head;
	count = 0;
	while (cur && count < 20)
	{
		printf("  [%d] type=%d (%s) value='%s' prev=%p next=%p\n", count++,
			cur->type,
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_WORD ? "WORD" : cur->type == TK_PIPE ? "PIPE" : cur->type == TK_REDIRECT_OUT ? ">" : cur->type == TK_REDIRECT_IN ? "<" : "OTHER",
			cur->value ? cur->value : "(null)", (void *)cur->prev,
			(void *)cur->next);
		cur = cur->next;
	}
	// Question: Where should parser start?
	printf("\n--- Parser Entry Point Analysis ---\n");
	// Option 1: Start from first token after HEAD
	cur = head->next;
	if (cur && cur->type != TK_EOF)
	{
		printf("Option 1 - Start from first after HEAD:\n");
		printf("  Token: type=%d value='%s'\n", cur->type,
			cur->value ? cur->value : "(null)");
	}
	// Option 2: Start from last command before EOF (rightmost)
	cur = head->next;
	if (cur && cur->type != TK_EOF)
	{
		while (cur->next && cur->next->type != TK_EOF)
		{
			// Only advance if next is still a valid token (not operator,
				not EOF)
			if (cur->next->type == TK_WORD || cur->next->type == TK_DOLLER)
				cur = cur->next;
			else
				break ;
		}
		printf("Option 2 - Start from rightmost command before EOF:\n");
		printf("  Token: type=%d value='%s'\n", cur->type,
			cur->value ? cur->value : "(null)");
	}
	// What does parse_command_list expect?
	printf("\n--- parse_command_list behavior ---\n");
	printf("Given starting token, it will:\n");
	printf("  1. Move RIGHT via ->next to find rightmost command\n");
	printf("  2. Then move LEFT via ->prev to process all commands\n");
	printf("  3. Stop when tmp->prev is not a command token\n");
	printf("  4. Set *cur_token = tmp (the left boundary)\n");
	// Try parsing
	printf("\n--- Actual Parse ---\n");
	// The parser should start from the last token before EOF
	// But NOT necessarily the rightmost command
	// It should start from EOF->prev (the last actual token)
	cur = head;
	while (cur->next && cur->next->type != TK_EOF)
		cur = cur->next;
	// Now cur is the last token before EOF
	printf("Starting from last token before EOF:\n");
	printf("  type=%d value='%s'\n", cur->type,
		cur->value ? cur->value : "(null)");
	root = NULL;
	fgen_tree(&root, &cur);
	printf("\nAfter parse:\n");
	printf("  cur_token type=%d (%s)\n", cur->type,
		cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : "OTHER");
	printf("  root=%p type=%d\n", (void *)root, root ? (int)root->type : -1);
	if (root && root->type == NODE_CMD && root->cmd)
	{
		arg = (t_argv *)root->cmd->argv;
		printf("  argv: [");
		while (arg)
		{
			printf("%s", arg->word);
			arg = arg->next;
			if (arg)
				printf(", ");
		}
		printf("]\n");
	}
}

int	main(void)
{
	analyze_flow("echo hello");
	analyze_flow("cat file.txt");
	analyze_flow("ls -la");
	printf("\n========================================\n");
	printf("CONCLUSION\n");
	printf("========================================\n");
	printf("The parser should start from:\n");
	printf("  EOF->prev (the last non-EOF token)\n");
	printf("\nThis allows parse_command_list to:\n");
	printf("  1. Move right to find rightmost command\n");
	printf("  2. Then process all commands left\n");
	printf("  3. Handle operators, redirections, etc.\n");
	return (0);
}
