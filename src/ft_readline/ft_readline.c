#include "../../includes/get_next_line.h"
#include "../../includes/minishell.h"

static int	process_key(char c, char *buf, size_t *len, t_readline_ctx *ctx)
{
	char	seq[2];

	if (c == 3)
		return (1);
	if (c == '\n')
		return (0);
	if (c == 4 && *len == 0)
		return (-1);
	if (c == 127)
		handle_backspace(buf, len);
	else if (c == 27)
	{
		if (read_key(&seq[0]) <= 0 || read_key(&seq[1]) <= 0)
			return (1);
		handle_escape_sequence(seq, ctx->hist, buf, ctx);
	}
	else if (c >= 32 && c <= 126)
		handle_printable(buf, len, c);
	return (1);
}

static char	*read_non_interactive(void)
{
	char	*buf;
	size_t	len;

	buf = get_next_line(STDIN_FILENO);
	if (!buf)
		return (NULL);
	len = ft_strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	return (buf);
}

static int	read_loop(char *buf, size_t *len, t_readline_ctx *ctx)
{
	char	c;
	int		result;

	while (read_key(&c) > 0)
	{
		result = process_key(c, buf, len, ctx);
		if (result == 0)
			return (0);
		else if (result == -1)
			return (-1);
	}
	return (0);
}

char	*ft_readline(const char *prompt, t_hist *hist)
{
	struct termios	orig;
	char			*buf;
	size_t			len;
	t_readline_ctx	ctx;

	if (!isatty(STDIN_FILENO))
		return (read_non_interactive());
	buf = ft_calloc(1024, 1);
	if (!buf)
		return (NULL);
	len = 0;
	ctx = (t_readline_ctx){&len, hist, prompt};
	enable_raw_mode(&orig);
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	if (read_loop(buf, &len, &ctx) == -1)
		return (disable_raw_mode(&orig), free(buf), NULL);
	write(STDOUT_FILENO, "\n", 1);
	disable_raw_mode(&orig);
	if (len > 0)
		add_history(buf, hist);
	return (buf);
}
