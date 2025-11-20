/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_identification.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:42:02 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 00:27:54 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	token_is_operator(t_token_type type)
{
	if (type == TK_AND_IF || type == TK_OR_IF || type == TK_PIPE)
		return (1);
	return (0);
}

int	token_is_subshell_close(t_token_type type)
{
	if (type == TK_RPAREN)
		return (1);
	return (0);
}

int	token_is_command(t_token_type type)
{
	if (type == TK_WORD || type == TK_DOLLER)
		return (1);
	return (0);
}

int	token_is_redirection(t_token_type type)
{
	if (type == TK_REDIRECT_IN || type == TK_REDIRECT_OUT || type == TK_HEREDOC
		|| type == TK_APPEND)
		return (1);
	return (0);
}

int	token_is_newline_or_eof(t_token_type type)
{
	if (type == TK_EOF)
		return (1);
	return (0);
}
