#include "../includes/minishell.h"

static void	exec_one_ast(t_ast *ast, t_shell *shell)
{
	t_result	res;

	if (!ast)
		return ;
	memset(&res, 0, sizeof(t_result));
	shell->root = ast;
	res = executor(ast, shell);
	// if (res)
	// {
	// 	shell->last_exit_status = res->exit_code;
	// 	free_result(res);
	// }
	shell->last_exit_status = res.exit_code;
	free_ast_tree(ast);
	shell->root = NULL;
}

static int	check_syntax_errors(t_token *token_list, t_shell *shell)
{
	t_token	*cur;

	cur = token_list->next;
	while (cur && cur->type != TK_EOF)
	{
		if (token_is_operator(cur->type) || token_is_redirection(cur->type))
		{
			if (!syntax_check(cur))
			{
				syntax_error(cur->type);
				shell->last_exit_status = 2;
				return (0);
			}
		}
		cur = cur->next;
	}
	return (1);
}

static t_token	*skip_to_command(t_token *token_list)
{
	t_token	*cur;

	cur = token_list;
	while (cur->type == TK_HEAD || cur->type == TK_NEWLINE)
	{
		cur = cur->next;
		if (cur->type == TK_EOF)
			return (NULL);
	}
	return (cur);
}

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_token	*cur;
	t_ast	*ast;
	int		paren_check;

	if (!token_list || !shell)
		return (0);
	paren_check = check_parenthesis_balance(token_list);
	if (paren_check > 0)
		return (write(2,
				"minishell: syntax error: unexpected EOF while looking for matching ')'\n",
				72), shell->last_exit_status = 2, 0);
	if (paren_check < 0)
		return (write(2, "minishell: syntax error near unexpected token ')'\n",
				50), shell->last_exit_status = 2, 0);
	if (!check_syntax_errors(token_list, shell))
		return (0);
	cur = skip_to_command(token_list);
	if (!cur)
		return (1);
	ast = parser(&cur);
	if (!ast)
		return (shell->last_exit_status = 2, 0);
	exec_one_ast(ast, shell);
	return (1);
}

static void	process_line(char *line, t_shell *shell, t_hist *hist)
{
	t_token	*token_list;

	if (shell->interactive)
		add_history(line, hist);
	token_list = lexer(line);
	shell->line_ptr = line;
	shell->token_list = token_list;
	parse_and_exec(token_list, shell);
	if (g_signum == SIGINT)
		write(1, "\n", 1);
	if (shell->token_list)
	{
		free_token_list(shell->token_list);
		shell->token_list = NULL;
	}
}

int	shell_loop(t_shell *shell)
{
	char			*line;
	char const		*prompt;
	static t_hist	hist;

	shell->hist = &hist;
	prompt = "minishell$ ";
	while (1)
	{
		g_signum = 0;
		line = ft_readline(prompt, &hist);
		if (g_signum == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signum = 0;
			xfree(line);
			continue ;
		}
		if (!line)
		{
			if (shell->interactive)
				printf("exit\n");
			break ;
		}
		if (line && *line == '\0')
		{
			xfree(line);
			continue ;
		}
		process_line(line, shell, &hist);
		xfree(line);
		shell->line_ptr = NULL;
	}
	return (0);
}

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
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
	set_variable(&shell, "_", "/usr/bin/minishell", 1);
	shell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}
