#include "../../../includes/minishell.h"

void	handle_backspace(char *buf, size_t *len)
{
	if (*len > 0)
	{
		(*len)--;
		buf[*len] = '\0';
		write(STDOUT_FILENO, "\b \b", 3);
	}
}

void	handle_printable(char *buf, size_t *len, char c)
{
	if (*len >= 1023)
		return ;
	buf[*len] = c;
	(*len)++;
	buf[*len] = '\0';
	write(STDOUT_FILENO, &c, 1);
}
