/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_t.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:39 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/25 00:03:14 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


void	free_token_list(t_token *p)
{
	if (p == NULL)
		return ;
    fprintf(stderr, "[FREE TOKEN] %p\n", p);
	free_token_list(p->next);
	xfree(p->value);
	xfree(p);
}

void	free_double_array(char **p)
{
	size_t	i;

	if (p == NULL || (p && p[0] == NULL))
		return ;
	i = 0;
	while (p[i] != NULL)
	{
		xfree(p[i]);
		i++;
	}
	xfree(p);
}

void	free_shell(t_shell *shell)
{
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
	if (shell->hist)
		free_hist_box(shell->hist->hist_box);
	free_env_list(shell->env_list);
	free_ast_tree(shell->root);
	free_token_list(shell->token_list);
	xfree(shell->line_ptr);
	xfree(shell->pwd);
}

void	free_env_list(t_env *env_list)
{
	if (env_list == NULL)
		return ;
	fprintf(stderr, "[FREE ENV_LIST] %p\n", env_list);
	free_env_list(env_list->next);
	xfree(env_list->key);
	xfree(env_list->value);
	xfree(env_list);
}
