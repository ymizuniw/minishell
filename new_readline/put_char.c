#include "new_readline.h"
//draw
int	putchar(int c)
{
	return ((int)write(STDOUT_FILENO, &c, 1));
}
