#include "new_readline.h"
//read
bool	read_char(char *one_char)
{
	ssize_t	rb;

	rb = read(STDIN_FILENO, one_char, 1);
	if (rb < 0)
	{
		perror("read");
		return (false);
	}
	return (true);
}
