#include "../../includes/minishell.h"

t_ast	*parser(t_token *dummy_head)
{
	t_token	*token;
	t_ast	*root;

	if (!dummy_head)
		return (NULL);
	root = NULL;
	token =  dummy_head->next;
	if (!token)
		return (NULL);
	root = gen_tree(NULL, &token, 0);
	if (!root)
	{
		assert(root==NULL);
		return (NULL);
	}
	while (token && token->type == TK_NEWLINE)
		token = token->next;
	dummy_head= token;
	return (root);
}
