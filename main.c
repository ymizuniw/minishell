/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:09 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 18:42:18 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_token	*cur;
	t_ast	*ast;

	if (!token_list || !shell)
		return (0);
	if (!check_parenthesis_errors(token_list, shell))
		return (0);
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

static void	process_line(char *line, t_shell *shell)
{
	t_token	*token_list;

	token_list = lexer(line);
	if (!token_list)
	{
		shell->last_exit_status = 1;
		shell->line_ptr = line;
		cleanup_after_line(shell);
		return ;
	}
	shell->line_ptr = line;
	shell->token_list = token_list;
	parse_and_exec(token_list, shell);
	if (g_signum == SIGINT)
		write(1, "\n", 1);
	cleanup_after_line(shell);
}

static int	handle_empty_line(char *line, t_shell *shell)
{
	if (!line)
	{
		if (shell->interactive)
			printf("exit\n");
		return (1);
	}
	if (*line == '\0')
	{
		xfree(line);
		return (1);
	}
	return (0);
}

int	shell_loop(t_shell *shell)
{
	char			*line;
	char const		*prompt;
	static t_hist	hist;

	shell->hist = &hist;
	hist.cur = -1;
	prompt = "minishell$ ";
	while (1)
	{
		g_signum = 0;
		line = ft_readline(shell, prompt, &hist);
		if (g_signum == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signum = 0;
			xfree(line);
			continue ;
		}
		if (handle_empty_line(line, shell))
		{
			if (!line)
				break ;
			continue ;
		}
		process_line(line, shell);
		xfree(line);
		shell->line_ptr = NULL;
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*pwd;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.stdin_backup = -1;
	shell.stdout_backup = -1;
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
