#include "new_readline.h"

int	process_signal(char one_char)
{
	if (one_char == ASC_ETX)
		return (1);
	else if (one_char == ASC_EOT)
		return (0);
	return (-1);
}

