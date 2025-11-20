/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:20 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/20 16:18:55 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// t_result	executor(t_ast *ast, t_shell *shell)
// {
// 	t_result	res;

// 	ft_memset(&res, 0, sizeof(t_result));
// 	if (!shell)
// 		return (res);
// 	ast_traversal(ast, shell);
// 	res.root = ast;
// 	res.exit_code = shell->last_exit_status;
// 	return (res);
// }

int	executor(t_ast *ast, t_shell *shell)
{
	if (!shell)
		return (-1);
	ast_traversal(ast, shell);
	shell->root = ast;
	return (1);
}
