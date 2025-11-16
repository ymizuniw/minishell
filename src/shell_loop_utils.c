/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:42:53 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/16 21:31:26 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*skip_to_command(t_token *token_list)
{
	t_token	*cur;

	cur = token_list;
	while (cur->type == TK_HEAD)
	{
		cur = cur->next;
		if (cur->type == TK_EOF)
			return (NULL);
	}
	return (cur);
}

int	check_syntax_errors(t_token *token_list, t_shell *shell)
{
	t_token	*cur;

	cur = token_list->next;
	while (cur && cur->type != TK_EOF)
	{
		if (token_is_operator(cur->type) || token_is_redirection(cur->type)
			|| cur->type == TK_LPAREN)
		{
			if (!syntax_check(cur))
			{
				syntax_error(cur->type);
				shell->last_exit_status = 2;
				return (0);
			}
		}
		cur = cur->next;
	}
	return (1);
}

int	check_parenthesis_errors(t_token *token_list, t_shell *shell)
{
	int	paren_check;

	paren_check = check_parenthesis_balance(token_list);
	if (paren_check > 0)
	{
		write(2, UNEXPECTED_EOF_MSG, 72);
		shell->last_exit_status = 2;
		return (0);
	}
	if (paren_check < 0)
	{
		write(2, UNEXPECTED_RPAREN_MSG, 50);
		shell->last_exit_status = 2;
		return (0);
	}
	return (1);
}

void	exec_one_ast(t_ast *ast, t_shell *shell)
{
	t_result	res;

	if (!ast)
		return ;
	ft_memset(&res, 0, sizeof(t_result));
	shell->root = ast;
	res = executor(ast, shell);
	shell->last_exit_status = res.exit_code;
	free_ast_tree(ast);
	shell->root = NULL;
}

void	cleanup_after_line(t_shell *shell)
{
	if (shell->token_list)
	{
		free_token_list(shell->token_list);
		shell->token_list = NULL;
	}
}
