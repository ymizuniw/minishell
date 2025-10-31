#include "../../includes/minishell.h"

void	free_ast_tree(t_ast *p)
{
	t_ast	*cur;

	cur = p;
	if (cur->left != NULL)
		free_ast_tree(cur->left);
	if (cur->right != NULL)
		free_ast_tree(cur->right);
	if (p->cmd != NULL)
		free_cmd_structure(p->cmd);
	if (p->subtree != NULL)
		free_subshell_tree(p->subtree);
	xfree(p);
}

void	free_subshell_tree(t_ast *p)
{
	free_ast_tree(p);
}

void	free_token_list(t_token *p)
{
	if (p->next != NULL)
		free_token_list(p->next);
	if (p->type != TK_WORD)
		xfree(p->value);
	xfree(p);
}

void	free_double_array_contents(char **p, char *p_content)
{
	int	i;

	if (!p)
		return ;
	if (p_content)
	{
		xfree(p_content);
		for (i = 0; p[i] != NULL; i++)
			p[i] = NULL;
		return ;
	}
	i = 0;
	while (p != NULL && p[i] != NULL)
	{
		xfree(p[i]);
		i++;
	}
}

void	free_double_array(char **p)
{
	if (p != NULL)
	{
		free_double_array_contents(p, NULL);
		xfree(p);
	}
}

void	free_redir_list(t_redir *p)
{
	if (p->next != NULL)
		free_redir_list(p->next);
	if (p->filename != NULL)
		xfree(p->filename);
	xfree(p);
}

void	free_cmd_structure(t_cmd *p)
{
	if (p->argv != NULL)
		free_double_array(p->argv);
	if (p->redir != NULL)
		free_redir_list(p->redir);
	xfree(p);
}

void	free_result(t_result *p)
{
	xfree(p);
}