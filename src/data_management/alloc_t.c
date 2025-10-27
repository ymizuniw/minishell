#include "../../includes/minishell.h"

// these may well have bzero?
t_ast	*alloc_node(void)
{
	return (t_ast *)xmalloc(sizeof(t_ast));
}

t_cmd	*alloc_cmd(void)
{
	return (t_cmd *)xmalloc(sizeof(t_cmd));
}

t_token	*alloc_token(void)
{
	return ((t_token *)xmalloc(sizeof(t_token)));
}

t_redir	*alloc_redir(void)
{
	return ((t_redir *)xmalloc(sizeof(t_redir)));
}

t_result	*alloc_result(void)
{
	return ((t_result *)xmalloc(sizeof(t_result)));
}
