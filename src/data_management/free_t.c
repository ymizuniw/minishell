#include "../../includes/minishell.h"

void	free_ast_tree(t_ast *p)
{
	t_ast	*cur;

	if (!p)
		return ;
	cur = p;
	if (cur->left == p || cur->right == p || cur->subtree == p)
		return ;
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
	if (p == NULL)
		return ;
	free_token_list(p->next);
	xfree(p->value);
	xfree(p);
}

void	free_double_array(char **p)
{
	size_t	i;

	if (p == NULL)
		return ;
	i = 0;
	while (p[i] != NULL)
	{
		xfree(p[i]);
		i++;
	}
	xfree(p);
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
	if (p->argv_list != NULL)
		free_argv_list(p->argv_list);
	xfree(p);
}

void	free_argv_list(t_argv *p)
{
	t_argv	*argv;

	if (p == NULL)
		return ;
	argv = p;
	if (argv->next)
		free_argv_list(argv->next);
	xfree(argv->word);
	xfree(argv);
}

void	free_result(t_result *p)
{
	xfree(p);
}

void	free_shell(t_shell *shell)
{
	free_env_list(shell->env_list);
	free_ast_tree(shell->root);
	free_token_list(shell->token_list);
	xfree(shell->line_ptr);
	xfree(shell->pwd);
}
