/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:19 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/24 23:30:07 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_ast_tree(t_ast *p)
{
	t_ast	*cur;

	if (!p)
		return ;
	cur = p;
    fprintf(stderr, "[FREE AST_TREE] %p\n", p);
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
    fprintf(stderr, "[FREE WORD_LIST] %p\n", p);
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
	fprintf(stderr, "[FREE REDIR_LIST] %p\n", p);
	free_redir_list(p->next);
	xclose(p->tmp_fd);
	xfree(p->filename);
	xfree(p);
}

void	free_cmd_structure(t_cmd *p)
{
	free_double_array(p->argv);
	if (p->redir != NULL)
		free_redir_list(p->redir);
	if (p->tokens != NULL)
		xfree(p->tokens);
	xfree(p);
}
