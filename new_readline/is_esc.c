#include "new_readline.h"

bool	is_esc(char one_char)
{
	return ((unsigned char)one_char == 0x1b);
}
