#include "../../../includes/minishell.h"

void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_lflag |= ECHOCTL;
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	disable_raw_mode(const struct termios *orig)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}

ssize_t	read_key(char *c)
{
	return (read(STDIN_FILENO, c, 1));
}

void	redraw_line(const char *prompt, const char *buf)
{
	write(STDOUT_FILENO, "\r\x1b[K", 4);
	write(STDOUT_FILENO, prompt, strlen(prompt));
	write(STDOUT_FILENO, buf, strlen(buf));
}
