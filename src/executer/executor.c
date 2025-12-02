/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:20 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 04:09:13 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	executor(t_ast *ast, t_shell *shell)
{
	if (!shell)
		return (-1);
	ast_traversal(ast, shell);
	shell->root = ast;
	return (1);
}
