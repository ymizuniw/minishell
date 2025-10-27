#include "../../../includes/minishell.h"
#include <errno.h>

#define INITIAL_PWD_BUF_SIZE 256
#define MAX_PWD_BUF_SIZE (64 * 1024)  // 64KB 上限.

char *get_pwd(void)
{
	size_t size;
	char *buf;

	buf = getcwd(NULL, 0);
	if (buf)
		return (buf);
	size = INITIAL_PWD_BUF_SIZE;
	while (size <= MAX_PWD_BUF_SIZE)
	{
		buf = xmalloc(size);
		if (!buf)
			return (NULL);
		if (getcwd(buf, size))
			return (buf);
		xfree(buf);
		if (errno != ERANGE)
			return (NULL);
		size *= 2;
	}
	return NULL;
}
