#include "../../../../includes/minishell.h"

// conduct redirection and execute command.
int	exec_command(t_ast *node, t_shell *shell)
{
	int		redir_ret;
	char	**expanded_argv;

	if (!node || !node->cmd)
		return (-1);
	redir_ret = do_redirection(node);
	if (redir_ret < 0)
	{
		if (node->cmd->redir && node->cmd->redir->filename)
			perror(node->cmd->redir->filename);
		return (1);
	}
	// Expand argv_list to char** just before execution
	if (!node->cmd->argv_list)
		return (0);
	expanded_argv = gen_argv(node->cmd->argv_list, shell);
	if (!expanded_argv || !expanded_argv[0])
	{
		if (expanded_argv)
			free(expanded_argv);
		return (0);
	}
	// Execute with expanded argv
	search_and_exec(shell, expanded_argv);
	// Clean up expanded argv
	for (int i = 0; expanded_argv[i]; i++)
		free(expanded_argv[i]);
	free(expanded_argv);
	return (0);
}
