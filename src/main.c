#include "../includes/minishell.h"

char	*ft_readline(char const *prompt, bool interactive)
{
	char	*line;
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	if (interactive == true)
	{
		line = readline(prompt);
		return (line);
	}
	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	if (nread > 0 && line[nread - 1] == '\n')
		line[nread - 1] = '\0';
	return (line);
}

static void	exec_one_ast(t_ast *ast, t_shell *shell)
{
	t_result	*res;

	if (!ast)
		return ;
	res = executor(ast, shell);
	if (res)
	{
		shell->last_exit_status = res->exit_code;
		free_result(res);
	}
	free_ast_tree(ast);
}

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_token	*cur;
	t_ast	*ast;

	if (!token_list || !shell)
		return (0);
	cur = token_list;
	while (cur && cur->type != TK_EOF)
	{
		if (cur->type == TK_HEAD || cur->type == TK_NEWLINE)
		{
			cur = cur->next;
			if (cur->type == TK_EOF)
				return (1);
			continue ;
		}
		// Parse one command, advancing cur to next unconsumed token
		ast = parser(&cur);
		if (ast)
			exec_one_ast(ast, shell);
		// cur now points to NEWLINE or EOF after parser advanced it
	}
	return (1);
}

int	shell_loop(t_shell *shell)
{
	char	*line;
	t_token	*token_list;

	line = NULL;
	token_list = NULL;
	while (1)
	{
		line = ft_readline("minishell$ ", shell->interactive);
		if (g_signum == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signum = 0;
		}
		if (!line)
		{
			if (shell->interactive)
				printf("exit\n");
			break ;
		}
		if (shell->interactive && *line)
			add_history(line);
		token_list = lexer(line);
		parse_and_exec(token_list, shell);
		xfree(line);
		if (token_list)
			free_token_list(token_list);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*pwd;

	(void)argc;
	(void)argv;
	memset(&shell, 0, sizeof(t_shell));
	if (isatty(STDIN_FILENO) == 1)
		shell.interactive = true;
	signal_initializer(shell.interactive);
	init_env_from_envp(&shell, env);
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		shell.pwd = pwd;
		set_variable(&shell, "PWD", pwd, 1);
	}
	shell.last_exit_status = 0;
	// Initialize _ to empty or to the shell path
	set_variable(&shell, "_", "/usr/bin/minishell", 1);
	shell_loop(&shell);
	if (shell.pwd)
		free(shell.pwd);
	free_env_list(shell.env_list);
	return (shell.last_exit_status);
}
