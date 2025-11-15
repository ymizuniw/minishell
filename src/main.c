/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 19:01:51 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 19:23:05 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		init_shell(t_shell *shell, char **env);
int			parse_and_exec(t_token *token_list, t_shell *shell);

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

static int	handle_signal_or_empty(char *line, t_shell *shell)
{
	if (g_signum == SIGINT)
	{
		shell->last_exit_status = 130;
		g_signum = 0;
		xfree(line);
		return (1);
	}
	if (handle_empty_line(line, shell))
		return (1);
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
		if (handle_signal_or_empty(line, shell))
		{
			if (!line && !g_signum)
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

	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	shell_loop(&shell);
	free_shell(&shell);
	return (shell.last_exit_status);
}
