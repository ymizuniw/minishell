#include "new_readline.h"

bool	is_signal(char one_char)
{
	return (one_char == ASC_ETX || one_char == ASC_EOT);
}
