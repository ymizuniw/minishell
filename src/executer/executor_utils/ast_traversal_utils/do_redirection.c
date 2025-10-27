#include "../../../../includes/minishell.h"

int	do_redirection(t_ast *node)
{
	t_redir	*cur;
	int		fd;

	if (!node || !node->cmd)
		return (-1);
	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN)
		{
			fd = open(cur->filename, O_RDONLY);
			if (fd < 0)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cur->type == REDIR_HEREDOC)
		{
			fd = make_heredoc(cur);
			if (fd < 0)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cur->type == REDIR_OUT)
		{
			fd = open(cur->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cur->type == REDIR_APPEND)
		{
			fd = open(cur->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (-1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		cur = cur->next;
	}
	return (1);
}
