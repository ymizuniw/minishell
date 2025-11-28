#include <term.h>
#include <sys/ioctl.h>
#include "../../../includes/minishell.h"


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
#define ANSI_UP "\x1b[1A"
#define ANSI_DOWN "\x1b[1B"
#define ANSI_RIGHT "\x1b[1C"
#define ANSI_LEFT "\x1b[1D"

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

int process_signal(one_char)
{
    if (one_char==ASC_ETX) 
        return (1);//interrupt.
    else if (one_char==ASC_EOT) 
        return (0);//end of input. if EOT && input_len==0, then exit, else do not anything.
}

bool is_del(char one_char)
{
    if (one_char=='\b')
        return (true);
    return (false);
}

int process_del(t_new_readline *nr, char one_char)
{
    char del_obj = nr->buf[nr->buf_len-1];
    size_t del_size = sizeof(del_obj);
    size_t count = 0;

    while (count<del_size)
    {
        tputs("\b", 1, ft_putchar);
        tputs(" ", 1, ft_putchar);
        tputs("\b", 1, ft_putchar);
        count++;
    }
    redraw_line(nr);
    return (1);
}

bool is_esc(char one_char)
{
    if (one_char=='\\')
        return (true);
    return (false);
}

int process_esc(t_new_readline *nr, char one_char)
{
    char esq_buf[2]={0};
    read_char(&esq_buf[0]);
    //read fail handling. [
    read_char(&esq_buf[1]);
    //read fail handling. A, B.
    if (ft_strcmp(esq_buf, ANSI_UP)==0)
        process_up(&nr);//hist up.
    else if (ft_strcmp(esq_buf, ANSI_DOWN==0))
        process_down(&nr);//hist down.
    else if (ft_strcmp(esq_buf, ANSI_RIGHT)==0)
        process_right(&nr);
    else if (ft_strcmp(esq_buf, ANSI_LEFT)==0)
        process_left(&nr);
}

int process_up(t_new_readline *nr)
{
    if (nr->hist->size==0)
        return (0);
    if (nr->hist->cur == -1)
        nr->hist->cur = (nr->hist->idx + HIST_MAX - 1) % HIST_MAX;
    else
        nr->hist->cur = (nr->hist->cur + HIST_MAX - 1) % HIST_MAX;
    if (nr->hist->hist_box[nr->hist->cur]==NULL)
        return ;
    copy_hist_to_buf(nr);
    redraw_line(nr);
}

int process_down(t_new_readline *nr)
{
    //
}

int process_right(t_new_readline *nr)
{
    if (nr->cursor_buf_pos + 1 < nr->buf_len)
    {
        nr->cursor_buf_pos++;
        nr->cursor_x++;
    }
    write(1, ANSI_RIGHT, 1);
    return (1);
}

int process_left(t_new_readline *nr)
{
    if (nr->cursor_buf_pos > 1)
    {
        nr->cursor_buf_pos--;
        nr->cursor_x--;
    }
    write(1, ANSI_LEFT, 1);
    return (1);
}

bool is_inserting(t_new_readline *nr)
{
    if (nr->cursor_buf_pos + 1 < nr->buf_len)
        return (true);
    return (false);
}

int insertion(t_new_readline *nr, char one_char)
{
    if (nr->buf_len==1023)//termination place kept?
        return (0);
    ft_memmove(nr->buf[nr->cursor_buf_pos + 1],nr->buf[nr->cursor_buf_pos], 1);
    nr->buf[nr->cursor_buf_pos] = one_char;
    return (1);
}

int write_to_buffer(t_new_readline *nr, char one_char)
{
    if (nr->buf_len>1023+1)
        return (0);
    else
        nr->buf[nr->cursor_buf_pos] = one_char;
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

char *new_readline(t_shell *shell)
{
    t_new_readline nr;
    ft_memset(&nr, 0, sizeof(t_new_readline));
    ft_memset(&nr.buf, 0, 1024);
    //keep tcgetattr() of current terminal.
    char one_char = 0;
    char *line=NULL;
    size_t line_len = 0;
    bool read_char_res = false;

    read_char_res = read_char(one_char);
    if (read_char_res==false)
    {
        printf("read_char failed.\n");
        return (NULL);
    }
    if (one_char == '\n')
        ;
    else if (is_signal(one_char))
    {
        if (process_signal(&nr,one_char)==0 && line_len==0)//EOT exit();
        {
            free_rsc();
            return (NULL);
        }
    }
    else if (is_del(one_char))
    {
        process_del(&nr,one_char);
    }
    else if (is_esc(one_char))
    {
        process_esc(&nr, one_char);
    }
    else if (ft_isprint(one_char) || ft_isspace(one_char))
        process_printable(&nr, one_char);
    return (line);
}
