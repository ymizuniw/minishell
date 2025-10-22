#include "../../includes/minishell.h"

char *path_validation(const char *cmd_name, char **env)
{
    if (command_name_with_no_slash)
    {
        if (search_built_in_command())
        {
            command_invoke();
        }
        else
        {
            if(search_path_of_the_name())
            {
                command_invoke();
            }
            else
            {
                find_command_not_found_handle();
            }
        }
    }
}
