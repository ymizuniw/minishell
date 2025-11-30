#include "new_readline.h"

bool	is_del(char one_char)
{
	return (one_char == '\b' || (unsigned char)one_char == 0x7f);
}
