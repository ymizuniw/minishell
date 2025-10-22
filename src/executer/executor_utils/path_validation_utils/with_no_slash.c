#include "../../../../includes/minishell.h"

//command name has slash '/' in its name or not.

bool with_no_slash(char *name)
{
    char *slash = strchr(name, '/');
    if (slash==NULL)
        return (true);
    return (false);
}
