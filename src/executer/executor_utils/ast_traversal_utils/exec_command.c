#include "../../../../includes/minishell.h"

// conduct redirection and execute command.
int	exec_command(t_ast *node, char **env)
{
	int		redir_ret;
	char	*path;
	int		execve_ret;

	if (!node || !node->cmd)
		return (1);
	redir_ret = do_redirection(node);
	if (redir_ret < 0)
	{
		if (node->cmd->redir_in && node->cmd->redir_in->filename)
			perror(node->cmd->redir_in->filename);
		else if (node->cmd->redir_out && node->cmd->redir_out->filename)
			perror(node->cmd->redir_out->filename);
		exit(redir_ret);
	}
	if (!node->cmd->argv || !node->cmd->argv[0])
		return (0);
	path = validate_path(node->cmd->argv[0], env);
	if (path == NULL)
		exit(127);
	execve_ret = execve(path, node->cmd->argv, env);
	if (execve_ret < 0)
		exit(1);
	return (0);
}
