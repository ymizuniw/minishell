#include "new_readline.h"

int	process_right(t_new_readline *nr)
{
	if (nr->cursor_buf_pos < nr->buf_len)
	{
		nr->cursor_buf_pos++;
		nr->cursor_x++;
		write(STDOUT_FILENO, ANSI_RIGHT, sizeof(ANSI_RIGHT) - 1);
	}
	return (1);
}
