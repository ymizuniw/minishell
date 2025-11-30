#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "new_readline.h"

int main(void)
{
    t_hist hist;
    memset(&hist, 0, sizeof(t_hist));

    char *line = NULL;
    while (1)
    {
        line = new_readline(&hist);
        if (!line)
        {
            printf("exit\n");
            break;
        }
        printf("READ: \"%s\"\n", line);
        add_history(line, &hist);
        free(line);
    }
    return 0;
}
