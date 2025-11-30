#include "new_readline.h"

int	write_to_buffer(t_new_readline *nr, char one_char)
{
	if (nr->buf_len >= sizeof(nr->buf) - 1)
		return (0);
	nr->buf[nr->buf_len] = one_char;
	nr->buf_len++;
	nr->cursor_buf_pos = nr->buf_len;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_x++;
	return (1);
}
