#include "new_readline.h"

int	process_left(t_new_readline *nr)
{
	if (nr->cursor_buf_pos > 0)
	{
		nr->cursor_buf_pos--;
		nr->cursor_x--;
		write(STDOUT_FILENO, ANSI_LEFT, sizeof(ANSI_LEFT) - 1);
	}
	return (1);
}
