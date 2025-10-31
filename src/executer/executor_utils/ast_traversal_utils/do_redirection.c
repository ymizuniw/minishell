#include "../../../../includes/minishell.h"

static int	do_each_redirection(const char *filename, int flags, mode_t perm,
		int io_fd)
{
	int	fd;

	if (!filename)
		return (-1);
	if (perm == 0)
		fd = open(filename, flags);
	else
		fd = open(filename, flags, perm);
	if (fd < 0)
		return (-1);
	if (dup2(fd, io_fd) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	do_redirect_hd(t_redir *hd)
{
	int	fd;

	fd = make_heredoc(hd);
	if (fd < 0)
		return (-1);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	do_redirection(t_ast *node)
{
	t_redir	*cur;

	if (!node || !node->cmd)
		return (-1);
	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN)
		{
			if (do_each_redirection(cur->filename, O_RDONLY, 0,
					STDIN_FILENO) < 0)
				return (-1);
		}
		else if (cur->type == REDIR_HEREDOC)
		{
			if (do_redirect_hd(cur) < 0)
				return (-1);
		}
		else if (cur->type == REDIR_OUT)
		{
			if (do_each_redirection(cur->filename, O_WRONLY | O_CREAT | O_TRUNC,
					0644, STDOUT_FILENO) < 0)
				return (-1);
		}
		else if (cur->type == REDIR_APPEND)
		{
			if (do_each_redirection(cur->filename,
					O_WRONLY | O_CREAT | O_APPEND, 0644, STDOUT_FILENO) < 0)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}
