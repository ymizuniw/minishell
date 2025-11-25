/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/26 00:25:43 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char *expand_plain_word(t_word *list, t_shell *shell);

static void	restore_stdio(int stdin_keep, int stdout_keep)
{
	dup2(stdin_keep, STDIN_FILENO);
	dup2(stdout_keep, STDOUT_FILENO);
	xclose(stdin_keep);
	xclose(stdout_keep);
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

// static void	free_expanded_argv(char **expanded_argv)
// {
// 	int	i;

// 	if (!expanded_argv)
// 		return ;
// 	i = 0;
// 	while (expanded_argv[i])
// 	{
// 		xfree(expanded_argv[i]);
// 		i++;
// 	}
// 	xfree(expanded_argv);
// }

static char **append_argv(char **argv, size_t *argc, char *str)
{
	char **new_argv = xmalloc(sizeof(char*)*(*argc+2));
	size_t i = 0;

	while (i< *argc)
	{
		new_argv[i] = argv[i];
		i++;
	}
	new_argv[*argc] = ft_strdup(str);
	new_argv[*argc + 1] = NULL;
	xfree(argv);
	(*argc)++;
	return (new_argv);
}

char **expand_token(t_token *token, t_shell *shell)
{
	size_t value_len;
	size_t additional;
	t_word *word_list;
	char **res;

	if (!token || !token->value)
		return (NULL);
	value_len = ft_strlen(token->value);
	additional = 0;
	word_list = gen_word(token->value, value_len, &additional);
	if (!word_list)
		return (NULL);
	if (has_wildcard_to_expand(word_list))
	{
		size_t count = 0;
		res  = expand_token_with_wildcard(word_list, shell, &count);
		free_word_list(word_list);
		return (res);
	}
	res = (char **)xcalloc(sizeof(char *) * 2);
	if (!res)
	{
		free_word_list(word_list);
		return (NULL);
	}
	res[0] = expand_plain_word(word_list, shell);
	res[1] = NULL;
	free_word_list(word_list);
	return (res);
}

char **expand_tokens(t_token **tokens, size_t count, t_shell *shell)
{
	char **argv = NULL;
	size_t argc = 0;

	for (size_t i = 0; i< count; i++)
	{
		char **expanded  = expand_token(tokens[i], shell);
		if (!expanded)
			return (free_double_array(argv), NULL);
		for (size_t j=0;expanded[j]; j++)
		{
			argv = append_argv(argv, &argc, expanded[j]);
		}
		free_double_array(expanded);
	}
	return (argv);
}

static int process_command(t_ast *node, t_shell *shell)
{
	char **argv = NULL;
	size_t argc = 0;

	for (size_t i = 0;i<node->cmd->token_count; i++)
	{
		char **expanded = expand_token(node->cmd->tokens[i], shell);
		for (size_t j = 0;expanded[j]; j++)
		{
			argv = append_argv(argv, &argc, expanded[j]);
		}
		free_double_array(expanded);
	}
	search_and_exec(shell, argv);
	free_double_array(argv);
	return (1);
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
