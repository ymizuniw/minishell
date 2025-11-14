#include "../../../../includes/minishell.h"

// int	ft_mkstmpfd(char *template, char **filename)
// {
// 	int	fd;

// 	(void)template;
// 	(void)filename;
// 	fd = open("/tmp", O_EXCL | __O_TMPFILE, 0600);
// 	if (fd >= 0)
// 		return (fd);
// 	return (-1);
// }

//for Mac book environment. __O_TMPFILE is not available.
int	ft_mkstmpfd(char *template, char **filename)
{
	int	fd;
	static int var_num;
	static int var_num2;
	unsigned long long rand_ptr;
	unsigned int rand_name;
	char *path;
	size_t limit=0;
	size_t count = 0;

	while (count < limit)
	{
		var_num = var_num + var_num2 % INT_MAX;
		rand_ptr = (unsigned long long)*(&var_num);
		(void)template;
		rand_name = (unsigned int)rand_ptr;
		*filename = ft_itoa(rand_name);
		path = ft_strjoin("tmp/", *filename);
		if (path==NULL)
			return (xfree(*filename),-1);
		printf("filename: %s\n", *filename);
		fd = open("/tmp/", O_EXCL | O_RDWR, 0600);
		if (fd >= 0)
			return (fd);
		unlink(path);
		count++;
	}
	return (-1);
}
