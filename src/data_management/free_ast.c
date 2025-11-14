#include "../../includes/minishell.h"

void	free_ast_tree(t_ast *p)
{
	t_ast	*cur;

	if (!p)
		return ;
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

void	free_word_list(t_word *p)
{
	t_word	*argv;

	if (p == NULL)
		return ;
	argv = p;
	if (argv->next)
		free_word_list(argv->next);
	xfree(argv->word);
	xfree(argv);
}

void	free_redir_list(t_redir *p)
{
	if (p == NULL)
		return ;
	free_redir_list(p->next);
	xfree(p->filename);
	xfree(p);
}

void	free_cmd_structure(t_cmd *p)
{
	free_double_array(p->argv);
	if (p->redir != NULL)
		free_redir_list(p->redir);
	// Note: tokens are owned by token_list, not by cmd
	// So we only free the tokens array, not the tokens themselves
	if (p->tokens != NULL)
		xfree(p->tokens);
	xfree(p);
}
