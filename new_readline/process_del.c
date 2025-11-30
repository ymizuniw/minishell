#include "new_readline.h"

int	process_del(t_new_readline *nr, char one_char)
{
	size_t	pos;

	(void)one_char;
	if (nr->buf_len == 0 || nr->cursor_buf_pos == 0)
		return (1);
	pos = nr->cursor_buf_pos - 1;
	ft_memmove(&nr->buf[pos], &nr->buf[pos + 1], nr->buf_len - pos - 1);
	nr->buf_len--;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_buf_pos--;
	redraw_line(nr);
	return (1);
}
