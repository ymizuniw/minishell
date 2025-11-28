#include "../../../includes/minishell.h"

#include <term.h>
#include <sys/ioctl.h>

//tgetent(): extract entry of terminal name to bp buffer.
//arg: bp == char buf[1024];

int tputs(const char *str, int affcnt, int (*putfunc)(int));
//str: control string to output.
//affcnt: the count of lines affected.ex>cursor move is 1.
//pubfunc: func pointer to output one character.

int ft_putchar(int c)
{
    write(STDOUT_FILENO, &c, 1);
}
char *tgoto(const char *cap, int col, int row);
//cap: pointer to string parameter got by tgetstr(...);
//col: destination colum.
//row: destination line.

int main(void)
{
    char buf[1024] = {0};
    char *term_type = getenv("TERM");
    if (term_type == NULL)
        return (1);
    int res = tgetent(buf, "TERM");
    if (res<0)
    {
        printf("no database fiel termcap was opened.\n");
        return (1);
    }
    else if (res==0)
    {
        printf("given terminal name is not entered.\n");
        return (1);
    }
    int height = tgetnum("li");
    if (height < 0)
    {
        printf("height couldn't get.\n");
        return (1);
    }
    int width = tgetnum("co");
    if (width<0)
    {
        printf("height couldn't get.\n");
        return (1);
    }
    printf("terminal width: %d\n", width);
    printf("terminal height: %d\n", height);
    printf("terminal size: %d\n", height*width);
    //tgetnum()
    char buffer[2048]={0};
    char *area = buffer;
    tgetent(buffer, "xterm");//xterm?
    char *clear_screen_seq = tgetstr("cl" , &area);
    if (clear_screen_seq!=NULL)
    {
        //do cls.
        //affcnt is height?
        tputs(clear_screen_seq, 1, ft_putchar);
    }
    else
    {
        printf("clear_screen_seq was not found.\n");
        return (1);
    }
    char *cursor_move_template = tgetstr("cm", &area);
    if (cursor_move_template!=NULL)
    {
       int target_row = 10;
       int target_col = 20;
       char *move_sequence = tgoto(cursor_move_template, target_col, target_row);
       if (move_sequence!=NULL)
       {
            tputs(move_sequence, 1, ft_putchar);
            printf("Hello, tgoto");
            int lines_l = tgetnum("li");
            char *move_to_bottom = tgoto(cursor_move_template, 0, lines_l -1);
            if (move_to_bottom)
            {
                tputs(move_to_bottom, 1, ft_putchar);
            }
       }
       else
            printf("cusor_sequence couldn't generated.\n");
    }
    else
    {
        printf("cursor_move_template was not found.\n");
        return (1);
    }
    printf("\n");
    return (0);
}

//window-size reallocating shall be captured by signal.
int main(void)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)>=0)
    {
        printf("Lines: %d\n", w.ws_row);
        printf("Colums: %d\n", w.ws_col);
    }
    else
    {
        perror("ioctl");
        return (1);
    }
    return (0);
}

int main(void)
{
    // handle_backspace();
    char input_char;
    scanf("%c", input_char);
    if (input_char=='\b')
    {
        tputs("\b", 1, ft_putchar);
        tputs(" ", 1, ft_putchar);
        tputs("\b", 1, ft_putchar);
    }
    return (0);
}
