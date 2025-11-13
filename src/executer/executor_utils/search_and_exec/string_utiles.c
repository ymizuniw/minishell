#include "../../../../includes/minishell.h"

char	*join_path(char *dir, char *cmd)
{
	char	*path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	return (path);
}

void	free_split(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
