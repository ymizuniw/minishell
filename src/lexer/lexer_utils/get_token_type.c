/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:21 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:39:22 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token_type	pipe_or_orif(char *input, size_t *idx);
t_token_type	redir_in_or_heredoc(char *input, size_t *idx);
t_token_type	redir_out_or_append(char *input, size_t *idx);
t_token_type	andif_or_word(char *input, size_t *idx);
t_token_type	handle_simple_tokens(char *input, size_t *idx);

t_token_type	get_token_type(char *input, size_t *idx)
{
	if (!input[*idx] || input[*idx] == '\n')
		return (TK_EOF);
	if (input[*idx] == '|')
		return (pipe_or_orif(input, idx));
	else if (input[*idx] == '<')
		return (redir_in_or_heredoc(input, idx));
	else if (input[*idx] == '>')
		return (redir_out_or_append(input, idx));
	else if (input[*idx] == '&')
		return (andif_or_word(input, idx));
	else
		return (handle_simple_tokens(input, idx));
}
