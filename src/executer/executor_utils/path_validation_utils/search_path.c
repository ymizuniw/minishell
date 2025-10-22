#include "../../../../includes/minishell.h"

//search not-built-in command path.

int search_path(char **argv, char **env)
{
    char *command_name = argv[0];
    char *path = getenv(command_name);
    if (path==NULL)
        return (NULL);
    else
    {
        argv[]
    }
}
