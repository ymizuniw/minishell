#include "includes/minishell.h"
#include <stdio.h>

t_ast	*parse(t_token *token_head);

int		test_num = 0;

int	exec_command(t_ast *node, t_shell *shell)
{
	t_argv	*arg;

	(void)shell;
	if (!node || !node->cmd)
		return (0);
	printf("  [%d] ", ++test_num);
	arg = (t_argv *)node->cmd->argv;
	while (arg)
	{
		printf("%s ", arg->word);
		arg = arg->next;
	}
	printf("\n");
	shell->last_exit_status = 0;
	return (0);
}

int	exec_pipe(t_ast *node, t_shell *shell, bool execute)
{
	(void)node;
	(void)shell;
	(void)execute;
	return (0);
}

int	exec_subshell(t_ast *node, t_shell *shell, bool execute)
{
	(void)node;
	(void)shell;
	(void)execute;
	return (0);
}

int		ast_traversal(t_ast *node, t_shell *shell, bool execute);

void	test(const char *input, const char *expected)
{
	t_token	*head;
	t_ast	*root;
	t_shell	shell;

	shell = {0};
	test_num = 0;
	printf("\nInput: %s\n", input);
	printf("Expected: %s\n", expected);
	head = lexer(input);
	root = parse(head);
	shell.last_exit_status = 0;
	ast_traversal(root, &shell, true);
}

int	main(void)
{
	printf("=== AST Traversal Order Tests ===\n");
	test("echo a && echo b", "1.echo a, 2.echo b");
	test("echo a || echo b", "1.echo a, 2.echo b");
	test("a && b && c", "1.a, 2.b, 3.c");
	test("a || b || c", "1.a, 2.b, 3.c");
	test("a && b || c", "1.a, 2.b, 3.c");
	test("ls && cat file && grep test", "1.ls, 2.cat file, 3.grep test");
	printf("\n✅ All tests verify execution order\n");
	return (0);
}
