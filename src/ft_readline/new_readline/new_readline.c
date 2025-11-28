#include <term.h>
#include <sys/ioctl.h>
#include "../../../includes/minishell.h"

typedef struct s_new_readline
{
    const char *prompt;
    size_t      prompt_len;

    char        buf[1024];//please init.
    size_t      buf_len;
    size_t      cursor_buf_pos;

    t_hist      *hist;

    size_t      cursor_x;
    size_t      cursor_y;
    size_t      terminal_width;
    size_t      terminal_height;
    struct termios raw_mode;
    struct termios original;
}t_new_readline;

//window resize signal_handler()
//to SIGWINCH
void handle_sigwinch(int signum)
{
    g_signum = signum;
}

int ft_putchar(int c)
{
    return (write(STDOUT_FILENO, &c, 1));
}

#define CLEAR_LINE "\r\x1b[K"
#define CLEAR_LINE_LEN 4
#define ANSI_UP "\x1b[A"
#define ANSI_DOWN "\x1b[B"
#define ANSI_RIGHT "\x1b[C"
#define ANSI_LEFT "\x1b[D"

//if explicit term_buffer is provided.
// char *buffer = (char *)malloc(ft_strlen(term_buffer));
// #define BUFFADDR &buffer

// cl_string = tgetstr("cl", BUFFADDR);
// cm_string = tgetstr("cm", BUFFADDR);
// auto_wrap = tgetstr("am");
// height = tgetnum("li");
// width = tgetnum("co");

//padding char for waiting tputs();
// temp = tgetstr("pc", BUFFADDR);
// PC = temp ? *temp : 0;
// BC = tgetstr("le", BUFFADDR);
// UP = tgetstr("up", BUFFADDR);

int redraw_line(t_new_readline *nr)
{
    write(STDOUT_FILENO, CLEAR_LINE, (size_t)CLEAR_LINE_LEN);
    write(STDOUT_FILENO,nr->prompt, nr->prompt_len);
    write(STDOUT_FILENO, nr->buf, nr->buf_len);

    size_t offset = nr->prompt_len + nr->cursor_buf_pos;
    size_t current = nr->prompt_len + nr->buf_len;
    while (current > offset)
    {
        write(STDOUT_FILENO, ANSI_LEFT, sizeof(ANSI_LEFT)-1);
        current--;
    }
    //cursor_x = prompt_len + 1;?
    return(1);
}

bool read_char(char *one_char)
{
    ssize_t rb = 0;

    rb = read(STDIN_FILENO,one_char,1);
    if (rb<0)
    {
        perror("read");
        return (false);
    }
    return (true);
}

bool is_signal(char one_char)
{
    if (one_char == ASC_ETX || one_char == ASC_EOT)
        return (true);
    return (false);
}

int process_signal(char one_char)
{
    if (one_char==ASC_ETX) 
        return (1);//interrupt.
    else if (one_char==ASC_EOT) 
        return (0);//end of input. if EOT && input_len==0, then exit, else do not anything.
    return(-1);
}

bool is_del(char one_char)
{
    if (one_char=='\b' || one_char == 0x7f)
        return (true);
    return (false);
}

int process_del(t_new_readline *nr, char one_char)
{
    if (nr->buf_len==0 || nr->cursor_buf_pos==0)
        return(0);
    size_t pos = nr->cursor_buf_pos - 1;
    ft_memmove(&nr->buf[pos], &nr->buf[pos + 1], nr->buf_len - pos -1);
    nr->buf_len--;
    nr->buf[nr->buf_len]='\0';
    nr->cursor_buf_pos--;
    redraw_line(nr);
    return (1);
}

bool is_esc(char one_char)
{
    if (one_char==0x1b)
        return (true);
    return (false);
}

#define CSI_UP   "[A"
#define CSI_DOWN "[B"
#define CSI_RIGHT "[C"
#define CSI_LEFT "[D"

int process_esc(t_new_readline *nr, char one_char)
{
    char esq_buf[3]={0};
    read_char(&esq_buf[0]);
    //read fail handling. [
    read_char(&esq_buf[1]);
    //read fail handling. A, B.
    if (ft_strcmp(esq_buf, CSI_UP)==0)
        process_up(nr);//hist up.
    else if (ft_strcmp(esq_buf, CSI_DOWN)==0)
        process_down(nr);//hist down.
    else if (ft_strcmp(esq_buf, CSI_RIGHT)==0)
        process_right(nr);
    else if (ft_strcmp(esq_buf, CSI_LEFT)==0)
        process_left(nr);
    return (1);
}

int process_up(t_new_readline *nr)
{
    if (!nr->hist || nr->hist->size==0)
        return (0);
    if (nr->hist->cur == -1)
        nr->hist->cur = (nr->hist->idx + HIST_MAX - 1) % HIST_MAX;
    else
        nr->hist->cur = (nr->hist->cur + HIST_MAX - 1) % HIST_MAX;
    if (nr->hist->hist_box[nr->hist->cur]==NULL)
        return (0);
    copy_history_to_buffer(nr);
    redraw_line(nr);
    return (1);
}

int process_down(t_new_readline *nr)
{
    if (!nr->hist || nr->hist->size == 0 || nr->hist->cur == -1)
        return 0;
    nr->hist->cur = (nr->hist->cur + 1) % HIST_MAX;
    if (nr->hist->cur == nr->hist->idx)
    {
        nr->buf[0] = '\0';
        nr->buf_len = 0;
        nr->cursor_buf_pos = 0;
        nr->hist->cur = -1;
    }
    else
    {
        copy_history_to_buffer(nr);
    }
    redraw_line(nr);
    return 1;
}

int process_right(t_new_readline *nr)
{
    if (nr->cursor_buf_pos < nr->buf_len)
    {
        nr->cursor_buf_pos++;
        nr->cursor_x++;
        write(1, ANSI_RIGHT, sizeof(ANSI_RIGHT) - 1);
    }
    return (1);
}

int process_left(t_new_readline *nr)
{
    if (nr->cursor_buf_pos > 0)
    {
        nr->cursor_buf_pos--;
        nr->cursor_x--;
        write(1, ANSI_LEFT, sizeof(ANSI_LEFT)-1);
    }
    return (1);
}

bool is_inserting(t_new_readline *nr)
{
    if (nr->cursor_buf_pos < nr->buf_len)
        return (true);
    return (false);
}

int insertion(t_new_readline *nr, char one_char)
{
    size_t second_len;
    if (nr->buf_len==1023)//termination place kept?
        return (0);
    second_len = nr->buf_len - nr->cursor_buf_pos;
    ft_memmove(&nr->buf[nr->cursor_buf_pos + 1], &nr->buf[nr->cursor_buf_pos], second_len);
    nr->buf[nr->cursor_buf_pos] = one_char;
    nr->buf_len++;
    nr->buf[nr->buf_len] = '\0';
    nr->cursor_buf_pos++;
    nr->cursor_x++;
    return (1);
}

int write_to_buffer(t_new_readline *nr, char one_char)
{
    if (nr->buf_len>=1023)
        return (0);
    nr->buf[nr->buf_len] = one_char;
    nr->buf_len++;
    nr->cursor_buf_pos = nr->buf_len;
    nr->buf[nr->buf_len] = '\0';
    nr->cursor_x++;
    return (1);
}

int process_printable(t_new_readline *nr, char one_char)
{
    //nr->cursor_x
    //nr->cursor_y
    int ret;
    if (is_inserting(nr))
        ret = insertion(nr, one_char);
    else
        ret = write_to_buffer(nr, one_char);
    redraw_line(nr);
    return (ret);
}

int resize_window(t_new_readline *nr)
{
    struct winsize w;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)>=0)
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

char *new_readline(t_shell *shell)
{
    t_new_readline nr;
    char one_char = 0;
    char *line = NULL;
    bool read_char_res = false;

    ft_memset(&nr, 0, sizeof(t_new_readline));
    ft_memset(&nr.buf, 0, sizeof(nr.buf));
    nr.hist = shell->hist;

    enable_raw_mode(&nr.original);

    nr.prompt = "minishell$";
    nr.prompt_len = ft_strlen(nr.prompt);
    write(STDOUT_FILENO, nr.prompt, nr.prompt_len);

    while (1)
    {
        read_char_res = read_char(&one_char);
        if (!read_char_res)
            break;
        if (one_char == '\n')
        {
            write(STDOUT_FILENO, "\n", 1);
            line = ft_strdup(nr.buf);
            break;
        }
        else if (is_signal(one_char))
        {
            int sig_res = process_signal(one_char);
            if (sig_res == 0)
            {
                if (nr.buf_len == 0)
                {
                    free_rsc();
                    line = NULL;
                }
                else
                {
                    line = ft_strdup(nr.buf);
                    write(STDOUT_FILENO, "\n", 1);
                }
                break;
            }
            else if (sig_res == 1)
            {
                nr.buf_len = 0;
                nr.buf[0] = '\0';
                write(STDOUT_FILENO, "\n", 1);
                line = ft_strdup("");
                break;
            }
        }
        else if (is_del(one_char))
            process_del(&nr, one_char);
        else if (is_esc(one_char))
            process_esc(&nr, one_char);
        else if (ft_isprint(one_char) || ft_isspace(one_char))
            process_printable(&nr, one_char);
    }
    disable_raw_mode(&nr.original);
    return line;
}

