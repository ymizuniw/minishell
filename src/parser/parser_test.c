#include "../../includes/minishell.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void	print_ast_safe(t_ast *ast, int depth, t_ast **visited,
		size_t visited_count)
{
	if (!ast)
		return ;
	// Detect already visited node
	for (size_t i = 0; i < visited_count; i++)
		if (visited[i] == ast)
		{
			for (int j = 0; j < depth; j++)
				printf("  ");
			printf("[Cycle detected -> node %p type %d]\n", (void *)ast,
				ast->type);
			return ;
		}
	visited[visited_count++] = ast;
	for (int i = 0; i < depth; i++)
		printf("  ");
	printf("Node type: %d", ast->type);
	if (ast->type == NODE_CMD && ast->cmd && ast->cmd->argv)
		printf(" (CMD) - argv[0]: %s", ast->cmd->argv[0]);
	else if (ast->type == NODE_PIPE)
		printf(" (PIPE)");
	else if (ast->type == NODE_AND)
		printf(" (AND)");
	else if (ast->type == NODE_OR)
		printf(" (OR)");
	else if (ast->type == NODE_SUBSHELL)
		printf(" (SUBSHELL)");
	printf("\n");
	if (ast->left)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("Left:\n");
		print_ast_safe(ast->left, depth + 1, visited, visited_count);
	}
	if (ast->right)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("Right:\n");
		print_ast_safe(ast->right, depth + 1, visited, visited_count);
	}
	if (ast->subtree)
	{
		for (int i = 0; i < depth; i++)
			printf("  ");
		printf("Subtree:\n");
		print_ast_safe(ast->subtree, depth + 1, visited, visited_count);
	}
}

void	print_ast(t_ast *ast, int depth)
{
	t_ast *visited[1024]; // generous limit
	print_ast_safe(ast, depth, visited, 0);
}

// // Helper function to print AST structure
// void	print_ast(t_ast *ast, int depth)
// {
// 	int	i;

// 	if (!ast)
// 		return ;
// 	for (i = 0; i < depth; i++)
// 		printf("  ");
// 	printf("Node type: %d", ast->type);
// 	if (ast->type == NODE_CMD && ast->cmd)
// 	{
// 		printf(" (CMD)");
// 		if (ast->cmd->argv && ast->cmd->argv[0])
// 			printf(" - argv[0]: %s", ast->cmd->argv[0]);
// 	}
// 	else if (ast->type == NODE_PIPE)
// 		printf(" (PIPE)");
// 	else if (ast->type == NODE_AND)
// 		printf(" (AND)");
// 	else if (ast->type == NODE_OR)
// 		printf(" (OR)");
// 	else if (ast->type == NODE_SUBSHELL)
// 		printf(" (SUBSHELL)");
// 	printf("\n");
// 	if (ast->left)
// 	{
// 		for (i = 0; i < depth; i++)
// 			printf("  ");
// 		printf("Left:\n");
// 		print_ast(ast->left, depth + 1);
// 	}
// 	if (ast->right)
// 	{
// 		for (i = 0; i < depth; i++)
// 			printf("  ");
// 		printf("Right:\n");
// 		print_ast(ast->right, depth + 1);
// 	}
// 	if (ast->subtree)
// 	{
// 		for (i = 0; i < depth; i++)
// 			printf("  ");
// 		printf("Subtree:\n");
// 		print_ast(ast->subtree, depth + 1);
// 	}
// }

void	test_simple_command(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 1: Simple command\n");
	input = "echo hello";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 1 completed\n\n");
}

void	test_pipe_command(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 2: Pipe command\n");
	input = "ls | grep test";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	// printf("\n===TOKEN_TYPE_VALIDATION===\n");
	// 	// TOKEN_TYPE VALIDATION
	// 	t_token *cur = tokens->next;
	// 	while (cur)
	// 	{
	// 		printf("token->type: %d\n", cur->type);
	// 		if (cur->next)
	// 			cur = cur->next;
	// 		else
	// 			break ;
	// 	}
	// 	printf("\n===NEXT_ITEM===\n");
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		printf("ast->left: %d\n", ast->left->type);
		if (ast->type == NODE_PIPE)
			printf("✓ Correctly identified as PIPE node\n");
		else
			printf("✗ Expected PIPE node, got type %d\n", ast->type);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 2 completed\n\n");
}

void	test_redirection(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 3: Redirection\n");
	input = "cat < input.txt";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		if (ast->type == NODE_CMD)
			printf("✓ Correctly identified as CMD node\n");
		else
			printf("✗ Expected CMD node, got type %d\n", ast->type);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 3 completed\n\n");
}

void	test_logical_and(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 4: Logical AND\n");
	input = "cmd1 && cmd2";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		if (ast->type == NODE_AND)
			printf("✓ Correctly identified as AND node\n");
		else
			printf("✗ Expected AND node, got type %d\n", ast->type);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 4 completed\n\n");
}

void	test_logical_or(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 5: Logical OR\n");
	input = "cmd1 || cmd2";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		if (ast->type == NODE_OR)
			printf("✓ Correctly identified as OR node\n");
		else
			printf("✗ Expected OR node, got type %d\n", ast->type);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 5 completed\n\n");
}

void	test_complex_pipeline(void)
{
	const char	*input;
	t_token		*tokens;
	t_ast		*ast;

	printf("Test 6: Complex pipeline\n");
	input = "cat file.txt | grep pattern | wc -l";
	printf("Input: '%s'\n", input);
	tokens = lexer(input);
	if (!tokens)
	{
		printf("✗ Lexer failed\n\n");
		return ;
	}
	ast = parser(tokens);
	if (ast)
	{
		printf("✓ Parser succeeded\n");
		print_ast(ast, 0);
		free_ast_tree(ast);
	}
	else
	{
		printf("✗ Parser returned NULL\n");
	}
	free_token_list(tokens);
	printf("✓ Test 6 completed\n\n");
}

int	main(void)
{
	printf("=================================\n");
	printf("  COMPREHENSIVE PARSER TESTS\n");
	printf("=================================\n\n");
	// test_simple_command();
	test_pipe_command();
	// test_redirection();
	// test_logical_and();
	// test_logical_or();
	// test_complex_pipeline();
	printf("=================================\n");
	printf("  ALL TESTS COMPLETED\n");
	printf("=================================\n");
	return (0);
}
