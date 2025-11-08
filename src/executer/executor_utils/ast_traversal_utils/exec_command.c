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

static int	process_command(t_ast *node, t_shell *shell, int *fds)
{
	char	**expanded_argv;

	if (!node->cmd || !node->cmd->argv_list || !node->cmd->argv_list->word)
		return (restore_stdio(fds[0], fds[1]), 0);
	expanded_argv = gen_argv(node->cmd->argv_list, shell);
	if (!expanded_argv || !expanded_argv[0])
		return (xfree(expanded_argv), 0);
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
	if (!node || !node->cmd)
		return (-1);
	redir_ret = do_redirection(node, shell);
	if (redir_ret < 0)
		return (handle_redirection_error(node, fds[0], fds[1], shell));
	process_command(node, shell, fds);
	restore_stdio(fds[0], fds[1]);
	return (0);
}
