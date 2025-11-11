#include "../../includes/minishell.h"

static void		enable_raw_mode(struct termios *orig);
static void		disable_raw_mode(const struct termios *orig);
// static void clear_line(void);
// static void move_cursor_left(size_t n);
// static void redraw_prompt(const char *prompt, const char *buf);
static ssize_t	read_key(char *c);
static void		handle_backspace(char *buf, size_t *len);
static void		handle_printable(char *buf, size_t *len, char c);
static void		handle_arrow_up(t_hist *);
static void		handle_arrow_down(t_hist *);

static void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO | ECHOCTL);
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

void	handle_arrow_up(t_hist *hist)
{
	(void)hist;
	printf("\n %s\n", "up");
}

void	handle_arrow_down(t_hist *hist)
{
	(void)hist;
	printf("\n %s\n", "down");
}

/* --- main readline-like function --- */
static void	handle_escape_sequence(char *seq, t_hist *hist)
{
	if (seq[0] == '[')
	{
		if (seq[1] == 'A')
			handle_arrow_up(hist);
		else if (seq[1] == 'B')
			handle_arrow_down(hist);
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
		handle_escape_sequence(seq, hist);
	}
	else if (c >= 32 && c <= 126)
		handle_printable(buf, len, c);
	return (1);
}

char	*ft_readline(const char *prompt, t_hist *hist)
{
	struct termios	orig;
	char			*buf;
	size_t			len;
	char			c;

	// Non-interactive mode: use getline()
	if (!isatty(STDIN_FILENO))
	{
		buf = NULL;
		len = 0;
		if (getline(&buf, &len, stdin) == -1)
		{
			if (buf)
				free(buf);
			return (NULL);
		}
		// Remove trailing newline
		len = strlen(buf);
		if (len > 0 && buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		return (buf);
	}
	// Interactive mode: use raw mode
	buf = calloc(1024, 1);
	len = 0;
	enable_raw_mode(&orig);
	write(STDOUT_FILENO, prompt, strlen(prompt));
	while (1)
	{
		if (read_key(&c) <= 0)
			break ;
		if (!process_key(c, buf, &len, hist))
			break ;
	}
	buf[len] = '\0';
	write(STDOUT_FILENO, "\n", 1);
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
