#include "../../includes/minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	ft_pwd(t_shell *shell, int fd)
{
	char	*cwd;

	(void)shell;
	// 常にgetcwd()で現在のディレクトリを取得
	cwd = get_pwd();
	if (!cwd)
	{
		write(2, "pwd: error retrieving current directory: ", 41);
		perror("getcwd");
		return ;
	}
	ft_putstr_fd(cwd, fd);
	ft_putchar_fd('\n', fd);
	xfree(cwd);
}
