#include "../../includes/minishell.h"

static void		enable_raw_mode(struct termios *orig);
static void		disable_raw_mode(const struct termios *orig);
// static void clear_line(void);
// static void move_cursor_left(size_t n);
// static void redraw_prompt(const char *prompt, const char *buf);
static ssize_t	read_key(char *c);
static void		handle_backspace(char *buf, size_t *len);
static void		handle_printable(char *buf, size_t *len, char c);
// static void		handle_arrow_up(t_hist *);
// static void		handle_arrow_down(t_hist *);

static void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

static void	disable_raw_mode(const struct termios *orig)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}

/* --- input helpers --- */
static ssize_t	read_key(char *c)
{
	return (read(STDIN_FILENO, c, 1));
}

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
	buf[*len] = c;
	(*len)++;
	buf[*len] = '\0';
	write(STDOUT_FILENO, &c, 1);
}

void	handle_arrow_up(t_hist *hist, char *buf, size_t *len)
{
	if (hist->size == 0)
		return ;
	if (hist->hist_box[hist->cur] == NULL)
		return ;
	if (hist->cur == -1)
		hist->cur = (hist->idx + HIST_MAX - 1) % HIST_MAX;
	else
		hist->cur = (hist->cur + HIST_MAX - 1) % HIST_MAX;
	strncpy(buf, hist->hist_box[hist->cur], 1023);
	buf[1023] = '\0';
	*len = strlen(buf);
	printf("\r");
	printf("\x1b[2K");
	printf("minishell$ %s", buf);
	fflush(stdout);
}

void	handle_arrow_down(t_hist *hist, char *buf, size_t *len)
{
	if (hist->size == 0 || hist->cur == -1)
		return ;
	if (hist->hist_box[hist->cur] == NULL)
		return ;
	hist->cur = (hist->cur + 1) % HIST_MAX;
	if ((size_t)hist->cur == hist->idx)
	{
		buf[0] = '\0';
		*len = 0;
		hist->cur = -1;
	}
	else if (hist->hist_box[hist->cur])
	{
		strncpy(buf, hist->hist_box[hist->cur], 1023);
		buf[1023] = '\0';
		*len = strlen(buf);
	}
	printf("\r");
	printf("\x1b[2K");
	printf("minishell$ %s", buf);
	fflush(stdout);
}

static void	handle_escape_sequence(char *seq, t_hist *hist, char *buf,
		size_t *len)
{
	if (seq[0] == '[')
	{
		if (seq[1] == 'A')
			handle_arrow_up(hist, buf, len);
		else if (seq[1] == 'B')
			handle_arrow_down(hist, buf, len);
	}
}

static int	process_key(char c, char *buf, size_t *len, t_hist *hist)
{
	char	seq[2];

	if (c == '\n')
		return (0);
	if (c == 127)
		handle_backspace(buf, len);
	else if (c == 27)
	{
		if (read_key(&seq[0]) == 0)
			return (1);
		if (read_key(&seq[1]) == 0)
			return (1);
		handle_escape_sequence(seq, hist, buf, len);
	}
	else if (c >= 32 && c <= 126)
		handle_printable(buf, len, c);
	return (1);
}

static char	*read_non_interactive(void)
{
	char	*buf;
	size_t	len;

	buf = NULL;
	len = 0;
	if (getline(&buf, &len, stdin) == -1)
	{
		free(buf);
		return (NULL);
	}
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	return (buf);
}

static char	*read_interactive_line(char *buf, size_t *len, t_hist *hist,
		struct termios *orig)
{
	char	c;

	while (1)
	{
		if (read_key(&c) <= 0)
			break ;
		if (c == 4)
		{
			if (*len == 0)
			{
				disable_raw_mode(orig);
				free(buf);
				return (NULL);
			}
			else
				continue ;
		}
		if (!process_key(c, buf, len, hist))
			break ;
	}
	return (buf);
}

char	*ft_readline(const char *prompt, t_hist *hist)
{
	struct termios	orig;
	char			*buf;
	size_t			len;

	if (!isatty(STDIN_FILENO))
		return (read_non_interactive());
	buf = calloc(1024, 1);
	len = 0;
	enable_raw_mode(&orig);
	write(STDOUT_FILENO, prompt, strlen(prompt));
	if (!read_interactive_line(buf, &len, hist, &orig))
		return (NULL);
	buf[len] = '\0';
	write(STDOUT_FILENO, "\n", 1);
	if (len > 0)
		add_history(buf, hist);
	disable_raw_mode(&orig);
	return (buf);
}

/* --- history --- */
void	add_history(char *line, t_hist *hist)
{
	if (hist->hist_box[hist->idx] != NULL)
		free(hist->hist_box[hist->idx]);
	hist->hist_box[hist->idx] = strdup(line);
	hist->idx = (hist->idx + 1) % HIST_MAX;
	if (hist->size < HIST_MAX)
		hist->size++;
	hist->cur = -1;
}

void	free_hist_box(char *hist_box[HIST_MAX])
{
	size_t	i;

	i = 0;
	while (i < HIST_MAX)
	{
		xfree(hist_box[i]);
		i++;
	}
}

// void handle_key_up(t_hist *hist)
// {
//     size_t cur_idx;

//     if (hist->size==0)
//         return ;
//     cur_idx = hist->size - 1;
//     //get_entry();
//     //return (address to the hist->box[size]);
//     //redraw_prompt
//     //memcpy(buf, entry, 1024);
//     //len = strlen(entry);
// }

// void handle_key_down(t_hist *hist)
// {

// }
