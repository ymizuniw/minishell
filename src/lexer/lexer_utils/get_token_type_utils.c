/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_type_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 11:44:42 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token_type	pipe_or_orif(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '|')
	{
		(*idx) += 2;
		return (TK_OR_IF);
	}
	else
	{
		(*idx) += 1;
		return (TK_PIPE);
	}
}

t_token_type	redir_in_or_heredoc(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '<')
	{
		(*idx) += 2;
		return (TK_HEREDOC);
	}
	else
	{
		(*idx) += 1;
		return (TK_REDIRECT_IN);
	}
}

t_token_type	redir_out_or_append(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '>')
	{
		(*idx) += 2;
		return (TK_APPEND);
	}
	else
	{
		(*idx) += 1;
		return (TK_REDIRECT_OUT);
	}
}

t_token_type	andif_or_word(char *input, size_t *idx)
{
	if (input[*idx + 1] && input[*idx + 1] == '&')
	{
		(*idx) += 2;
		return (TK_AND_IF);
	}
	else
	{
		(*idx) += 1;
		return (TK_WORD);
	}
}

t_token_type	handle_simple_tokens(char *input, size_t *idx)
{
	if (input[*idx] == '(')
	{
		(*idx)++;
		return (TK_LPAREN);
	}
	else if (input[*idx] == ')')
	{
		(*idx)++;
		return (TK_RPAREN);
	}
	else if (input[*idx] == '$')
	{
		(*idx)++;
		return (TK_DOLLER);
	}
	return (TK_WORD);
}
