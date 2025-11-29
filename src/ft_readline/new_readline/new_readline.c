#include <term.h>
#include <sys/ioctl.h>
#include "../../../includes/minishell.h"

#include <termcap.h>

void init_termcap(void)
{
    char *term = getenv("TERM");
    static char term_buffer[2048];

    if (!term)
        term = "xterm-256color";
    if (tgetent(term_buffer, term) < 0)
    {
        perror("tgetent");
        exit(1);
    }
}

/**
 * @brief termcap を使った折り返し対応 redraw（完全版）
 *
 * - cm (cursor motion)
 * - ce (clear to end of line)
 * - cd (clear to end of screen)
 *
 * を利用し、プロンプト＋入力全体を再描画する。
 *
 * カーソル位置は cm で正確に指定。
 *
 * @param nr new_readline 構造体
 * @return 1
 */
static int redraw_line(t_new_readline *nr)
{
    char *cm;       /* cursor movement capability */
    char *ce;       /* clear to end of line */
    char *cd;       /* clear to end of screen */

    size_t total;       /* prompt + buffer 全体の文字数 */
    size_t cursor_pos;  /* cursor absolute logical position */
    size_t row, col;
    size_t total_rows;
    size_t i;
    /* termcap capability を取得 */
    cm = tgetstr("cm", NULL);
    ce = tgetstr("ce", NULL);
    cd = tgetstr("cd", NULL);
    if (!cm || !ce || !cd)
        return (1);
    if (nr->terminal_width == 0)
        nr->terminal_width = 80; /* fallback */
    /* ---------- 計算 ---------- */
    total = nr->prompt_len + nr->buf_len;
    cursor_pos = nr->prompt_len + nr->cursor_buf_pos;
    total_rows = total / nr->terminal_width + 1;
    row = cursor_pos / nr->terminal_width;
    col = cursor_pos % nr->terminal_width;
    /* ---------- (1) 一旦カーソルを一番上（このプロンプト行の先頭）へ ---------- */
    {
        char *goto_top = tgoto(cm, 0, 0);
        tputs(goto_top, 1, ft_putchar);
    }
    /* ---------- (2) 必要行数分をクリア ---------- */
    for (i = 0; i < total_rows; i++)
    {
        tputs(ce, 1, ft_putchar);   /* 行末クリア */
        if (i + 1 < total_rows)
            write(STDOUT_FILENO, "\n", 1); /* 下へ進む */
    }
    /* 最終行の先頭に戻す */
    {
        char *goto_top = tgoto(cm, 0, 0);
        tputs(goto_top, 1, ft_putchar);
    }
    /* ---------- (3) prompt + 入力文字列をメモリ通り書く ---------- */
    {
        write(STDOUT_FILENO, nr->prompt, nr->prompt_len);

        /* 折り返し考慮して連続出力 */
        write(STDOUT_FILENO, nr->buf, nr->buf_len);
    }
    /* ---------- (4) カーソル位置復元 ---------- */
    {
        char *goto_cursor = tgoto(cm, col, row);
        tputs(goto_cursor, 1, ft_putchar);
    }
    return (1);
}

/**
 * @brief 折り返し対応: カーソルを左へ1文字分動かす (termcap使用)
 *
 * @param nr new_readline 構造体
 * @return int always 1
 */
static int process_left(t_new_readline *nr)
{
    char *cm;

    if (nr->cursor_buf_pos == 0)
        return (1); /* これ以上左に行けない */

    nr->cursor_buf_pos--;

    /* termcap でカーソル移動 */
    cm = tgetstr("cm", NULL);
    if (!cm)
        return (1);

    size_t pos = nr->prompt_len + nr->cursor_buf_pos;
    size_t row = pos / nr->terminal_width;
    size_t col = pos % nr->terminal_width;

    char *goto_xy = tgoto(cm, col, row);
    tputs(goto_xy, 1, ft_putchar);

    return (1);
}

/**
 * @brief 折り返し対応: カーソルを右へ1文字分動かす (termcap使用)
 *
 * @param nr new_readline 構造体
 * @return int always 1
 */
static int process_right(t_new_readline *nr)
{
    char *cm;

    if (nr->cursor_buf_pos >= nr->buf_len)
        return (1);

    nr->cursor_buf_pos++;

    cm = tgetstr("cm", NULL);
    if (!cm)
        return (1);

    size_t pos = nr->prompt_len + nr->cursor_buf_pos;
    size_t row = pos / nr->terminal_width;
    size_t col = pos % nr->terminal_width;

    char *goto_xy = tgoto(cm, col, row);
    tputs(goto_xy, 1, ft_putchar);

    return (1);
}

void	add_history(char *line, t_hist *hist)
{
	if (!line || !*line || hist == NULL)
		return ;
	if (hist->hist_box[hist->idx] != NULL)
		xfree(hist->hist_box[hist->idx]);
	hist->hist_box[hist->idx] = ft_strdup(line);
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
		hist_box[i] = NULL;
		i++;
	}
}

void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	disable_raw_mode(const struct termios *orig)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}


void	handle_sigwinch(int signum)
{
	g_signum = signum;
}


int	ft_putchar(int c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

#define CLEAR_LINE "\r\x1b[K"
#define CLEAR_LINE_LEN 4
#define ANSI_UP "\x1b[A"
#define ANSI_DOWN "\x1b[B"
#define ANSI_RIGHT "\x1b[C"
#define ANSI_LEFT "\x1b[D"

static void	copy_history_to_buffer(t_new_readline *nr);

static int	redraw_line(t_new_readline *nr)
{
	size_t	offset;
	size_t	current;

	write(STDOUT_FILENO, CLEAR_LINE, (size_t)CLEAR_LINE_LEN);
	write(STDOUT_FILENO, nr->prompt, nr->prompt_len);
	write(STDOUT_FILENO, nr->buf, nr->buf_len);
	offset = nr->prompt_len + nr->cursor_buf_pos;
	current = nr->prompt_len + nr->buf_len;
	while (current > offset)
	{
		write(STDOUT_FILENO, ANSI_LEFT, sizeof(ANSI_LEFT) - 1);
		current--;
	}
	return (1);
}

static bool	read_char(char *one_char)
{
	ssize_t	rb;

	rb = read(STDIN_FILENO, one_char, 1);
	if (rb < 0)
	{
		perror("read");
		return (false);
	}
	return (true);
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

static bool	is_signal_char(char c)
{
	return (c == ASC_ETX || c == ASC_EOT);
}

static int	process_signal(t_new_readline *nr, char c)
{
	if (c == ASC_ETX)
	{
		nr->buf_len = 0;
		nr->buf[0] = '\0';
		nr->cursor_buf_pos = 0;
		write(STDOUT_FILENO, "^C\n", 3);
		write(STDOUT_FILENO, nr->prompt, nr->prompt_len);
		return (2);
	}
	if (c == ASC_EOT)
		return (0);
	return (-1);
}

static bool	is_del(char one_char)
{
	return (one_char == '\b' || one_char == 0x7f);
}

static int	process_del(t_new_readline *nr, char one_char)
{
	size_t	pos;

	(void)one_char;
	if (nr->buf_len == 0 || nr->cursor_buf_pos == 0)
		return (0);
	pos = nr->cursor_buf_pos - 1;
	ft_memmove(&nr->buf[pos], &nr->buf[pos + 1], nr->buf_len - pos - 1);
	nr->buf_len--;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_buf_pos--;
	redraw_line(nr);
	return (1);
}

static bool	is_esc(char one_char)
{
	return (one_char == 0x1b);
}

#define CSI_UP   "[A"
#define CSI_DOWN "[B"
#define CSI_RIGHT "[C"
#define CSI_LEFT "[D"

static int	process_up(t_new_readline *nr)
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

static int	process_down(t_new_readline *nr)
{
	if (!nr->hist || nr->hist->size == 0 || nr->hist->cur == -1)
		return (0);
	nr->hist->cur = (nr->hist->cur + 1) % HIST_MAX;
	if (nr->hist->cur == nr->hist->idx)
	{
		nr->buf[0] = '\0';
		nr->buf_len = 0;
		nr->cursor_buf_pos = 0;
		nr->hist->cur = -1;
	}
	else
		copy_history_to_buffer(nr);
	redraw_line(nr);
	return (1);
}

static int	process_right(t_new_readline *nr)
{
	if (nr->cursor_buf_pos < nr->buf_len)
	{
		nr->cursor_buf_pos++;
		nr->cursor_x++;
		write(1, ANSI_RIGHT, sizeof(ANSI_RIGHT) - 1);
	}
	return (1);
}

static int	process_left(t_new_readline *nr)
{
	if (nr->cursor_buf_pos > 0)
	{
		nr->cursor_buf_pos--;
		nr->cursor_x--;
		write(1, ANSI_LEFT, sizeof(ANSI_LEFT) - 1);
	}
	return (1);
}

static int	process_esc(t_new_readline *nr, char one_char)
{
	char	esq_buf[3];

	(void)one_char;
	ft_memset(esq_buf, 0, sizeof(esq_buf));
	if (!read_char(&esq_buf[0]))
		return (1);
	if (!read_char(&esq_buf[1]))
		return (1);
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

static bool	is_inserting(t_new_readline *nr)
{
	return (nr->cursor_buf_pos < nr->buf_len);
}

static int	insertion(t_new_readline *nr, char one_char)
{
	size_t	second_len;

	if (nr->buf_len >= 1023)
		return (0);
	second_len = nr->buf_len - nr->cursor_buf_pos;
	ft_memmove(&nr->buf[nr->cursor_buf_pos + 1],
		&nr->buf[nr->cursor_buf_pos], second_len);
	nr->buf[nr->cursor_buf_pos] = one_char;
	nr->buf_len++;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_buf_pos++;
	nr->cursor_x++;
	return (1);
}

static int	write_to_buffer(t_new_readline *nr, char one_char)
{
	if (nr->buf_len >= 1023)
		return (0);
	nr->buf[nr->buf_len] = one_char;
	nr->buf_len++;
	nr->cursor_buf_pos = nr->buf_len;
	nr->buf[nr->buf_len] = '\0';
	nr->cursor_x++;
	return (1);
}

static int	process_printable(t_new_readline *nr, char one_char)
{
	int	ret;

	if (is_inserting(nr))
		ret = insertion(nr, one_char);
	else
		ret = write_to_buffer(nr, one_char);
	redraw_line(nr);
	return (ret);
}

static int	resize_window(t_new_readline *nr)
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

static void	copy_history_to_buffer(t_new_readline *nr)
{
	size_t	hist_len;
	char	*src;

	if (!nr->hist || nr->hist->cur < 0)
		return ;
	src = nr->hist->hist_box[nr->hist->cur];
	if (!src)
		return ;
	hist_len = ft_strlen(src);
	if (hist_len > sizeof(nr->buf) - 1)
		hist_len = sizeof(nr->buf) - 1;
	ft_strlcpy(nr->buf, src, sizeof(nr->buf));
	nr->buf_len = hist_len;
	nr->cursor_buf_pos = hist_len;
}

char	*new_readline(t_shell *shell)
{
	t_new_readline	nr;
	char			one_char;
	char			*line;
	bool			read_char_res;

	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (read_non_interactive());
	ft_memset(&nr, 0, sizeof(t_new_readline));
	ft_memset(&nr.buf, 0, sizeof(nr.buf));
	nr.hist = shell->hist;
	nr.prompt = "minishell$ ";
	nr.prompt_len = ft_strlen(nr.prompt);
	line = NULL;
	one_char = 0;
	read_char_res = false;
	enable_raw_mode(&nr.original);
	resize_window(&nr);
	write(STDOUT_FILENO, nr.prompt, nr.prompt_len);
	while (1)
	{
		if (g_signum == SIGWINCH)
		{
			resize_window(&nr);
			redraw_line(&nr);
			g_signum = 0;
		}
		read_char_res = read_char(&one_char);
		if (!read_char_res)
			break;
		if (one_char == '\n')
		{
			write(STDOUT_FILENO, "\n", 1);
			line = ft_strdup(nr.buf);
			break;
		}
		else if (is_signal_char(one_char))
		{
			int	sig_res = process_signal(&nr, one_char);

			if (sig_res == 2)
				continue ;
			if (sig_res == 0)
			{
				if (nr.buf_len == 0)
				{
					free_rsc();
					line = NULL;
					break;
				}
				else
					continue ;
			}
		}
		else if (is_del(one_char))
			process_del(&nr, one_char);
		else if (is_esc(one_char))
			process_esc(&nr, one_char);
		else if (ft_isprint((int)one_char) || ft_isspace((int)one_char))
			process_printable(&nr, one_char);
	}
	disable_raw_mode(&nr.original);
	if (line && shell && shell->hist && nr.buf_len > 0)
		add_history(line, shell->hist);
	return (line);
}
