#include "new_readline.h"

int	process_up(t_new_readline *nr)
{
	if (!nr->hist || nr->hist->size == 0)
		return (0);
	if (nr->hist->cur == -1)
		nr->hist->cur = (nr->hist->idx + HIST_MAX - 1) % HIST_MAX;
	else
		nr->hist->cur = (nr->hist->cur + HIST_MAX - 1) % HIST_MAX;
	if (nr->hist->hist_box[nr->hist->cur] == NULL)
		return (0);
	copy_history_to_buffer(nr);
	redraw_line(nr);
	return (1);
}
