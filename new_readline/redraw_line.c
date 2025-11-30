
#include "new_readline.h"

int	redraw_line(t_new_readline *nr)
{
	size_t	offset;
	size_t	current;

	write(STDOUT_FILENO, CLEAR_LINE, (size_t)CLEAR_LINE_LEN);
	write(STDOUT_FILENO, nr->prompt, nr->prompt_len);
	write(STDOUT_FILENO, nr->buf, nr->buf_len);
	offset = nr->prompt_len + nr->cursor_buf_pos;
	current = nr->prompt_len + nr->buf_len;
	while (current > offset)
	{
		write(STDOUT_FILENO, ANSI_LEFT, sizeof(ANSI_LEFT) - 1);
		current--;
	}
	return (1);
}
