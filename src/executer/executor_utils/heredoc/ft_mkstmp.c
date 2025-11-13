#include "../../../../includes/minishell.h"

int	ft_mkstmpfd(char *template, char **filename)
{
	int	fd;

	(void)template;
	(void)filename;
	fd = open("/tmp", O_EXCL | __O_TMPFILE, 0600);
	if (fd >= 0)
		return (fd);
	return (-1);
}
