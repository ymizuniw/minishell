#include "../../../../includes/minishell.h"

int	exec_subshell(t_ast *node, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		set_sig_dfl();
		if (node->cmd && node->cmd->redir)
		{
			if (do_redirection(node, shell) < 0)
				exit(shell->last_exit_status);
		}
		if (node && node->subtree)
			ast_traversal(node->subtree, shell);
		exit(shell->last_exit_status);
	}
	handle_child(&shell->last_exit_status, pid);
	return (0);
}
