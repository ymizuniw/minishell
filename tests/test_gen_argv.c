#include "../includes/minishell.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Helper function to create t_argv node
static t_argv	*create_argv_node(const char *word, bool to_expand)
{
	t_argv	*node;

	node = malloc(sizeof(t_argv));
	if (!node)
		return (NULL);
	node->word = strdup(word);
	node->to_expand = to_expand;
	node->next = NULL;
	return (node);
}

// Helper function to free argv list
static void	free_argv_list(t_argv *list)
{
	t_argv	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->word);
		free(tmp);
	}
}

// Helper function to free char **argv
static void	free_argv_array(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

// Helper to create test environment
static t_env	*create_test_env(void)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	env->key = strdup("HOME");
	env->value = strdup("/home/testuser");
	env->exported = true;
	env->next = malloc(sizeof(t_env));
	env->next->key = strdup("USER");
	env->next->value = strdup("testuser");
	env->next->exported = true;
	env->next->next = malloc(sizeof(t_env));
	env->next->next->key = strdup("PATH");
	env->next->next->value = strdup("/usr/bin:/bin");
	env->next->next->exported = true;
	env->next->next->next = NULL;
	return (env);
}

static void	free_test_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

// Test 1: NULL input returns NULL
static void	test_null_input(void)
{
	t_shell	shell;
	char	**result;

	shell.env_list = NULL;
	shell.last_exit_status = 0;
	result = gen_argv(NULL, &shell);
	assert(result == NULL);
	printf("✓ Test 1 passed: NULL input returns NULL\n");
}

// Test 2: Single word without expansion
static void	test_single_word_no_expand(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = NULL;
	shell.last_exit_status = 0;
	list = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	printf("✓ Test 2 passed: Single word without expansion\n");
}

// Test 3: Multiple words without expansion
static void	test_multiple_words_no_expand(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = NULL;
	shell.last_exit_status = 0;
	// Note: Parser builds list in reverse order
	list = create_argv_node("world", false);
	list->next = create_argv_node("hello", false);
	list->next->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "hello") == 0);
	assert(result[2] != NULL);
	assert(strcmp(result[2], "world") == 0);
	assert(result[3] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	printf("✓ Test 3 passed: Multiple words without expansion (order reversed)\n");
}

// Test 4: Variable expansion - $HOME
static void	test_variable_expansion_home(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	list = create_argv_node("$HOME", true);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "/home/testuser") == 0);
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 4 passed: Variable expansion $HOME\n");
}

// Test 5: Variable expansion - $?
static void	test_variable_expansion_exit_status(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = NULL;
	shell.last_exit_status = 42;
	list = create_argv_node("$?", true);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "42") == 0);
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	printf("✓ Test 5 passed: Variable expansion $?\n");
}

// Test 6: Mixed expansion and non-expansion
static void	test_mixed_expansion(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	// Parser order: last to first
	list = create_argv_node("$USER", true);
	list->next = create_argv_node("world", false);
	list->next->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "world") == 0);
	assert(result[2] != NULL);
	assert(strcmp(result[2], "testuser") == 0);
	assert(result[3] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 6 passed: Mixed expansion and non-expansion\n");
}

// Test 7: Undefined variable returns empty string
static void	test_undefined_variable(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	list = create_argv_node("$UNDEFINED", true);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "") == 0);
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 7 passed: Undefined variable returns empty string\n");
}

// Test 8: Word with $ but to_expand=false (single quotes)
static void	test_dollar_no_expand(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	list = create_argv_node("$HOME", false);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "$HOME") == 0); // Should NOT be expanded
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 8 passed: Dollar sign without expansion (single quotes)\n");
}

// Test 9: Multiple variables in one word
static void	test_multiple_variables_in_word(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	list = create_argv_node("$USER:$HOME", true);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	printf("    Expected: 'testuser:/home/testuser'\n");
	printf("    Got:      '%s'\n", result[1]);
	assert(strcmp(result[1], "testuser:/home/testuser") == 0);
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 9 passed: Multiple variables in one word\n");
}

// Test 10: Variable with literal text
static void	test_variable_with_literal(void)
{
	t_shell	shell;
	t_argv	*list;
	char	**result;

	shell.env_list = create_test_env();
	shell.last_exit_status = 0;
	list = create_argv_node("prefix$USER-suffix", true);
	list->next = create_argv_node("echo", false);
	result = gen_argv(list, &shell);
	assert(result != NULL);
	assert(result[0] != NULL);
	assert(strcmp(result[0], "echo") == 0);
	assert(result[1] != NULL);
	assert(strcmp(result[1], "prefixtestuser-suffix") == 0);
	assert(result[2] == NULL);
	free_argv_list(list);
	free_argv_array(result);
	free_test_env(shell.env_list);
	printf("✓ Test 10 passed: Variable with literal text\n");
}

int	main(void)
{
	printf("\n=== Running gen_argv Unit Tests ===\n\n");
	test_null_input();
	test_single_word_no_expand();
	test_multiple_words_no_expand();
	test_variable_expansion_home();
	test_variable_expansion_exit_status();
	test_mixed_expansion();
	test_undefined_variable();
	test_dollar_no_expand();
	test_multiple_variables_in_word();
	test_variable_with_literal();
	printf("\n=== All tests passed! ===\n\n");
	return (0);
}
