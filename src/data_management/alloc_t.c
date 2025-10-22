#include "../../includes/minishell.h"

//these may well have bzero?
t_ast *alloc_node(void)
{
    return (t_ast *)malloc(sizeof(t_ast));
}

t_cmd *alloc_cmd(void)
{
    return (t_cmd *)malloc(sizeof(t_cmd));
}

t_token *alloc_token(void)
{
    return ((t_token *)malloc(sizeof(t_token)));
}

t_redir *alloc_redir(void)
{
    return ((t_redir *)malloc(sizeof(t_redir)));
}

t_pipeline *alloc_pipeline(void)
{
    return ((t_pipeline *)malloc(sizeof(t_pipeline)));
}
