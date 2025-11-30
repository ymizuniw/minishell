#include "new_readline.h"

bool	is_inserting(t_new_readline *nr)
{
	return (nr->cursor_buf_pos < nr->buf_len);
}
