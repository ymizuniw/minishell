#include "../../../includes/minishell.h"

char **gen_argv(t_word *word_list, t_shell *shell)
{
    //prepare char **argv for command args.

    //1. expand doller.
    //2. expand wildcard.
    //this is totally expanded char * 's array.
    char **expanded_arr = ft_expand_word(word_list, shell->env_list);
    if (expanded_arr==NULL)
        return (NULL);
    return (expanded_arr);
}
