/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 20:49:12 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_token	*cur;
	t_ast	*ast;

	if (!token_list || !shell)
		return (0);
	if (!check_parenthesis_errors(token_list, shell))
		return (0);
	if (!check_syntax_errors(token_list, shell))
		return (0);
	cur = skip_to_command(token_list);
	if (!cur)
		return (1);
	ast = parser(&cur);
	if (!ast)
		return (shell->last_exit_status = 2, 0);
	if (process_all_heredoc(shell, ast) < 0)
		return (shell->last_exit_status = 1, 0);
	exec_one_ast(ast, shell);
	return (1);
}
