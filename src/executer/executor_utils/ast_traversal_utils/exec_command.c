/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:37:16 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static void	restore_stdio(int stdin_keep, int stdout_keep)
{
	dup2(stdin_keep, STDIN_FILENO);
	dup2(stdout_keep, STDOUT_FILENO);
	close(stdin_keep);
	close(stdout_keep);
}

static int	handle_redirection_error(t_ast *node, int stdin_k, int stdout_k,
		t_shell *shell)
{
	if (node->cmd->redir && node->cmd->redir->filename)
		perror(node->cmd->redir->filename);
	shell->last_exit_status = 1;
	restore_stdio(stdin_k, stdout_k);
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	return (1);
}

static void	free_expanded_argv(char **expanded_argv)
{
	int	i;

	if (!expanded_argv)
		return ;
	i = 0;
	while (expanded_argv[i])
	{
		xfree(expanded_argv[i]);
		i++;
	}
	xfree(expanded_argv);
}

static int	process_command(t_ast *node, t_shell *shell)
{
	char	**expanded_argv;

	if (!node->cmd || !node->cmd->tokens || node->cmd->token_count == 0)
		return (0);
	expanded_argv = ft_expand_word(node->cmd->tokens, node->cmd->token_count,
			shell);
	if (!expanded_argv || !expanded_argv[0])
	{
		xfree(expanded_argv);
		return (0);
	}
	search_and_exec(shell, expanded_argv);
	free_expanded_argv(expanded_argv);
	return (0);
}

int	exec_command(t_ast *node, t_shell *shell)
{
	int	redir_ret;
	int	fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	shell->stdin_backup = fds[0];
	shell->stdout_backup = fds[1];
	if (!node || !node->cmd)
	{
		restore_stdio(fds[0], fds[1]);
		shell->stdin_backup = -1;
		shell->stdout_backup = -1;
		return (-1);
	}
	redir_ret = do_redirection(node, shell);
	if (redir_ret < 0)
		return (handle_redirection_error(node, fds[0], fds[1], shell));
	process_command(node, shell);
	restore_stdio(fds[0], fds[1]);
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	return (0);
}
