#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "minishell.h"

// テスト用のt_argv作成ヘルパー
t_argv	*create_argv_node(const char *word, bool to_expand, bool to_wildcard_expand)
{
	t_argv	*node;

	node = malloc(sizeof(t_argv));
	if (!node)
		return (NULL);
	node->word = strdup(word);
	node->to_expand = to_expand;
	node->to_wildcard_expand = to_wildcard_expand;
	node->next = NULL;
	return (node);
}

// t_argvリストに追加
void	append_argv(t_argv **head, t_argv *new)
{
	t_argv	*cur;

	if (!*head)
	{
		*head = new;
		return;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

// t_argvリストを解放
void	free_argv_list(t_argv *head)
{
	t_argv	*next;

	while (head)
	{
		next = head->next;
		free(head->word);
		free(head);
		head = next;
	}
}

// テスト用のt_shell作成
t_shell	*create_test_shell(void)
{
	t_shell	*shell;
	char	cwd[1024];

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	memset(shell, 0, sizeof(t_shell));
	
	if (getcwd(cwd, sizeof(cwd)))
		shell->pwd = strdup(cwd);
	else
		shell->pwd = strdup(".");
	
	return (shell);
}

// t_shellを解放
void	free_test_shell(t_shell *shell)
{
	if (!shell)
		return;
	free(shell->pwd);
	free(shell);
}

// 結果を表示
void	print_argv(char **argv)
{
	int	i;

	if (!argv)
	{
		printf("argv is NULL\n");
		return;
	}
	printf("Generated argv:\n");
	i = 0;
	while (argv[i])
	{
		printf("  argv[%d] = \"%s\"\n", i, argv[i]);
		i++;
	}
	printf("  argv[%d] = NULL\n", i);
	printf("Total arguments: %d\n", i);
}

int main(void)
{
	t_shell	*shell;
	t_argv	*argv_list;
	char	**result;

	printf("=== Test 1: Simple arguments (no wildcard) ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("ls", false, false));
	append_argv(&argv_list, create_argv_node("-la", false, false));
	append_argv(&argv_list, create_argv_node("/tmp", false, false));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 2: Wildcard expansion (*.c) ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("cat", false, false));
	append_argv(&argv_list, create_argv_node("*.c", false, true));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 3: Multiple wildcards ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("echo", false, false));
	append_argv(&argv_list, create_argv_node("*.c", false, true));
	append_argv(&argv_list, create_argv_node("*.h", false, true));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 4: Wildcard with prefix/suffix (test*.txt) ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("ls", false, false));
	append_argv(&argv_list, create_argv_node("test*.txt", false, true));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 5: No match wildcard ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("echo", false, false));
	append_argv(&argv_list, create_argv_node("*.nonexistent123", false, true));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 6: Mixed normal and wildcard ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("gcc", false, false));
	append_argv(&argv_list, create_argv_node("-Wall", false, false));
	append_argv(&argv_list, create_argv_node("*.c", false, true));
	append_argv(&argv_list, create_argv_node("-o", false, false));
	append_argv(&argv_list, create_argv_node("program", false, false));
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 7: Wildcard disabled (literal *) ===\n");
	shell = create_test_shell();
	argv_list = NULL;
	append_argv(&argv_list, create_argv_node("echo", false, false));
	append_argv(&argv_list, create_argv_node("*.c", false, false));  // to_wildcard_expand = false
	
	result = gen_argv(argv_list, shell);
	print_argv(result);
	free_double_array(result);
	free_argv_list(argv_list);
	free_test_shell(shell);

	printf("\n=== Test 8: NULL input ===\n");
	shell = create_test_shell();
	result = gen_argv(NULL, shell);
	print_argv(result);
	free_test_shell(shell);

	printf("\n=== All tests completed ===\n");
	return (0);
}
