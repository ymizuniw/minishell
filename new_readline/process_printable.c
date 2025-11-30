#include "new_readline.h"

//process_
int	process_printable(t_new_readline *nr, char one_char)
{
	int	ret;

	if (is_inserting(nr))
		ret = insertion(nr, one_char);
	else
		ret = write_to_buffer(nr, one_char);
	redraw_line(nr);
	return (ret);
}
