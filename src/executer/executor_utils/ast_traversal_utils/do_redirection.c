#include "../../../../includes/minishell.h"

char	*get_unique_filename(void)
{
	// TODO: Implement unique filename generation for heredoc
	return (NULL);
}

int	do_redirection(t_ast *node)
{
	t_redir	*cur;
	int		fd;

	if (!node || !node->cmd)
		return (0);
	cur = node->cmd->redir_in;
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
			// TODO: Implement heredoc handling
			cur->filename = get_unique_filename();
			if (!cur->filename)
				return (-1);
			fd = open(cur->filename, O_WRONLY | O_APPEND | O_EXCL);
			if (fd < 0)
				return (-1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		cur = cur->next;
	}
	cur = node->cmd->redir_out;
	while (cur != NULL)
	{
		if (cur->type == REDIR_OUT)
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
	return (0);
}
