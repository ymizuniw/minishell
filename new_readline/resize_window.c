#include "new_readline.h"

int	resize_window(t_new_readline *nr)
{
	struct winsize	w;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) >= 0)
	{
		nr->terminal_width = w.ws_col;
		nr->terminal_height = w.ws_row;
	}
	else
	{
		perror("ioctl");
		return (-1);
	}
	return (1);
}
