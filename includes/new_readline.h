#ifndef NEW_READLINE_H
#define NEW_READLINE_H

#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define HIST_MAX 100
#define ASC_ETX  3   // Ctrl-C
#define ASC_EOT  4   // Ctrl-D

typedef struct s_hist
{
    char    *hist_box[HIST_MAX];
    int     idx;
    int     size;
    int     cur;
}   t_hist;

typedef struct s_new_readline
{
    const char      *prompt;
    size_t           prompt_len;

    char             buf[1024];
    size_t           buf_len;
    size_t           cursor_buf_pos;

    size_t           cursor_x;
    size_t           cursor_y;

    size_t           terminal_width;
    size_t           terminal_height;

    struct termios   original;
    t_hist          *hist;
}   t_new_readline;

char *new_readline(t_hist *hist, bool is_heredoc, const char *hd_prompt);

void    add_history(char *line, t_hist *hist);
void    free_hist_box(char *hist_box[HIST_MAX]);
void    copy_history_to_buffer(t_new_readline *nr);

/* signal */
void    handle_sigwinch(int signum);

/* termios */
void    enable_raw_mode(struct termios *orig);
void    disable_raw_mode(const struct termios *orig);

/* drawing */
int     redraw_line(t_new_readline *nr);

/* input handlers */
bool    read_char(char *one_char);
bool    is_signal(char one_char);
int     process_signal(char one_char);

bool    is_del(char one_char);
int     process_del(t_new_readline *nr, char one_char);

bool    is_esc(char one_char);
int     process_esc(t_new_readline *nr, char one_char);

int     process_up(t_new_readline *nr);
int     process_down(t_new_readline *nr);
int     process_right(t_new_readline *nr);
int     process_left(t_new_readline *nr);

bool    is_inserting(t_new_readline *nr);
int     insertion(t_new_readline *nr, char one_char);
int     write_to_buffer(t_new_readline *nr, char one_char);

int     process_printable(t_new_readline *nr, char one_char);

/* window */
int     resize_window(t_new_readline *nr);

#endif
