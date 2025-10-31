#include "includes/minishell.h"
#include <stdio.h>

t_ast	*parse(t_token *token_head);

// Mock execution to test traversal order
int	exec_command(t_ast *node, t_shell *shell)
{
	t_argv	*arg;

	(void)shell;
	if (!node || !node->cmd)
		return (0);
	printf("EXEC: ");
	arg = (t_argv *)node->cmd->argv;
	while (arg)
	{
		printf("%s ", arg->word);
		arg = arg->next;
	}
	printf("\n");
	return (0);
}

int	exec_pipe(t_ast *node, t_shell *shell)
{
	t_argv	*arg;
	t_argv	*arg;

	(void)shell;
	printf("PIPE: right=");
	if (node->right && node->right->type == NODE_CMD)
	{
		arg = (t_argv *)node->right->cmd->argv;
		while (arg)
		{
			printf("%s ", arg->word);
			arg = arg->next;
		}
	}
	printf("| left=");
	if (node->left && node->left->type == NODE_CMD)
	{
		arg = (t_argv *)node->left->cmd->argv;
		while (arg)
		{
			printf("%s ", arg->word);
			arg = arg->next;
		}
	}
	printf("\n");
	return (0);
}

int	exec_subshell(t_ast *node, t_shell *shell)
{
	(void)node;
	(void)shell;
	return (0);
}

int		ast_traversal(t_ast *node, t_shell *shell);

int	main(void)
{
	t_token	*head;
	t_ast	*root;
	t_shell	shell;

	memset(&shell, 0, sizeof(t_shell));
	printf("=== Test 1: echo a && echo b ===\n");
	printf("Expected order: 'echo a' THEN 'echo b'\n");
	head = lexer("echo a && echo b");
	root = parse(head);
	shell.last_exit_status = 0;
	ast_traversal(root, &shell);
	printf("\n=== Test 2: a && b && c ===\n");
	printf("Expected order: 'a' THEN 'b' THEN 'c'\n");
	head = lexer("a && b && c");
	root = parse(head);
	shell.last_exit_status = 0;
	ast_traversal(root, &shell);
	return (0);
}
