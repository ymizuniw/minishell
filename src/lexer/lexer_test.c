#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	print_tokens(t_token *head)
{
	t_token	*current;
	int		count;

	current = head->next;
	count = 0;
	printf("\n--- Token List ---\n");
	while (current)
	{
		printf("Token %d: Type=%d", count++, current->type);
		if (current->value)
			printf(", Value='%s'", current->value);
		printf("\n");
		current = current->next;
	}
	printf("Total tokens: %d\n\n", count);
}

void	test_simple_command(void)
{
	const char	*input = "echo hello world";
	t_token		*tokens;

	tokens = lexer(input);
	assert(tokens != NULL);
	printf("Test 1: Simple command\n");
	printf("Input: '%s'\n", input);
	print_tokens(tokens);
	free_token_list(tokens);
	printf("✓ Test 1 passed\n\n");
}

void	test_pipe(void)
{
	const char	*input = "ls | grep test";
	t_token		*tokens;

	tokens = lexer(input);
	assert(tokens != NULL);
	printf("Test 2: Pipe command\n");
	printf("Input: '%s'\n", input);
	print_tokens(tokens);
	free_token_list(tokens);
	printf("✓ Test 2 passed\n\n");
}

void	test_redirects(void)
{
	const char	*input;
	t_token		*tokens;
	t_token		*current;
	int			token_count;

	input = "cat < input > output";
	tokens = lexer(input);
	assert(tokens != NULL);
	printf("Test 3: Redirections\n");
	printf("Input: '%s'\n", input);
	// First, just display all tokens to debug
	current = tokens->next;
	token_count = 0;
	printf("\n--- Token List ---\n");
	while (current)
	{
		printf("Token %d: Type=%d", token_count, current->type);
		if (current->value)
			printf(", Value='%s'", current->value);
		printf("\n");
		token_count++;
		current = current->next;
	}
	printf("Total tokens: %d\n\n", token_count);
	free_token_list(tokens);
	printf("✓ Test 3 passed (displayed %d tokens)\n\n", token_count);
}

void	test_quotes(void)
{
	const char	*input = "echo \"hello world\" 'test'";
	t_token		*tokens;

	tokens = lexer(input);
	assert(tokens != NULL);
	printf("Test 4: Quoted strings\n");
	printf("Input: '%s'\n", input);
	print_tokens(tokens);
	free_token_list(tokens);
	printf("✓ Test 4 passed\n\n");
}

void	test_logical_operators(void)
{
	const char	*input;
	t_token		*tokens;

	printf("Test 5: Logical operators\n");
	// Test 5a: AND operator
	input = "cmd1 && cmd2";
	printf("Input 5a: '%s'\n", input);
	tokens = lexer(input);
	assert(tokens != NULL);
	print_tokens(tokens);
	free_token_list(tokens);
	// Test 5b: OR operator
	input = "cmd1 || cmd2";
	printf("Input 5b: '%s'\n", input);
	tokens = lexer(input);
	assert(tokens != NULL);
	print_tokens(tokens);
	free_token_list(tokens);
	printf("✓ Test 5 passed\n\n");
}

void	test_parentheses(void)
{
	const char	*input = "(echo hello) && (echo world)";
	t_token		*tokens;

	// const char *input = "(cat)";
	tokens = lexer(input);
	assert(tokens != NULL);
	printf("Test 6: Parentheses\n");
	printf("Input: '%s'\n", input);
	print_tokens(tokens);
	free_token_list(tokens);
	printf("✓ Test 6 passed\n\n");
}

static void	run_case(const char *input)
{
	t_token	*tokens;
	t_token	*cur;

	tokens = lexer(input);
	assert(tokens != NULL);
	// quick pass to ensure EOF at end
	cur = tokens;
	while (cur->next)
		cur = cur->next;
	assert(cur->type == TK_EOF);
	free_token_list(tokens);
}

int	main(void)
{
	printf("=================================\n");
	printf("    LEXER TEST SUITE\n");
	printf("=================================\n\n");
	test_simple_command();
	test_pipe();
	test_redirects();
	test_quotes();
	test_logical_operators();
	test_parentheses();
	printf("=================================\n");
	printf("  ALL TESTS PASSED! ✓\n");
	printf("=================================\n");
	printf("Lexer quick tests\n");
	run_case("");
	run_case("echo hello world\n");
	run_case("ls | grep x && echo ok\n");
	run_case("(a)||b\n");
	run_case("echo $USER > out\n");
	printf("ok\n");
	return (0);
}
