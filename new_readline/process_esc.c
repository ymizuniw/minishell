#include "new_readline.h"

int	process_esc(t_new_readline *nr, char one_char)
{
	char	esq_buf[3] = {0};

	(void)one_char;
	if (!read_char(&esq_buf[0]))
		return (0);
	if (!read_char(&esq_buf[1]))
		return (0);
	if (ft_strcmp(esq_buf, CSI_UP) == 0)
		process_up(nr);
	else if (ft_strcmp(esq_buf, CSI_DOWN) == 0)
		process_down(nr);
	else if (ft_strcmp(esq_buf, CSI_RIGHT) == 0)
		process_right(nr);
	else if (ft_strcmp(esq_buf, CSI_LEFT) == 0)
		process_left(nr);
	return (1);
}
