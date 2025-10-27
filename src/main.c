#include "../includes/minishell.h"

char	*ft_readline(char const *prompt, bool interactive)
{
	char	*line;

	line = NULL;
	if (interactive == true)
	{
		line = readline(prompt);
		return (line);
	}
	// line = get_next_line(0);
	return (line);
}

int	shell_loop(t_shell *shell)
{
	char		*line;
	t_token		*token_list;
	t_ast		*ast;
	t_result	*res;

	line = NULL;
	token_list = NULL;
	ast = NULL;
	while (1)
	{
		line = ft_readline("minishell$ ", shell->interactive);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (shell->interactive && *line)
			add_history(line);

		token_list = lexer(line);

		t_ast **ast_list = NULL;
		size_t ast_count = 0;
		t_token *cur = token_list;
		while (cur && cur->type != TK_EOF)
		{
			ast = parser(&cur);
			if (!ast)
				continue;
			ast_list = realloc(ast_list, sizeof(t_ast *) * (ast_count + 1));
			ast_list[ast_count++] = ast;
			while (cur && cur->type == TK_NEWLINE)
				cur = cur->next;
		}
		size_t i = 0;
		while (i < cur->count_newline)
		{
			res = executor(ast_list[i], shell);
			free_ast_tree(ast_list[i]);
			free_result(res);
			i++;
		}
		xfree(line);
		if (token_list)
			free_token_list(token_list);
		xfree(ast_list);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	memset(&shell, 0, sizeof(t_shell));
	if (isatty(STDIN_FILENO) == 1)
		shell.interactive = true;
	signal_initializer(shell.interactive);
	init_env_from_envp(&shell, env);
	shell_loop(&shell);
	free_env_list(shell.env_list);
	return (0);
}
