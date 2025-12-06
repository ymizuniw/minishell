/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_command_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 17:27:59 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#define SYNTAX_REDIR_TARGET_NOT_FOUND "minishell: \
syntax error: missing redirection target\n"
#define SYNTAX_REDIR_TARGET_NOT_FOUND_LEN 53

void	set_redir_type(t_redir *new_redir, t_token_type token_type)
{
	if (token_type == TK_REDIRECT_IN)
		new_redir->type = REDIR_IN;
	else if (token_type == TK_REDIRECT_OUT)
		new_redir->type = REDIR_OUT;
	else if (token_type == TK_HEREDOC)
		new_redir->type = REDIR_HEREDOC;
	else
		new_redir->type = REDIR_APPEND;
}

void	append_redir(t_redir **redir_head, t_redir *new_redir)
{
	redir_add_back(redir_head, new_redir);
}

t_token	*find_end_token(t_token *cur_token)
{
	t_token	*tmp;

	tmp = cur_token;
	while (tmp->next && (token_is_command(tmp->next->type)
			|| token_is_redirection(tmp->next->type)))
		tmp = tmp->next;
	return (tmp->next);
}

int	handle_redirection_parse(t_cmd *cmd, t_token *tmp)
{
	if (!syntax_check(tmp))
	{
		syntax_error(tmp->type);
		return (-1);
	}
	if (!tmp->next)
		return (-1);
	if (!tmp->prev)
	{
		write(2, SYNTAX_REDIR_TARGET_NOT_FOUND,
			SYNTAX_REDIR_TARGET_NOT_FOUND_LEN);
		return (-1);
	}
	return (parse_redirection(&cmd->redir, tmp->type, tmp->prev));
}

int	process_token(t_cmd *cmd, t_token *tmp)
{
	if (token_is_redirection(tmp->type))
		return (handle_redirection_parse(cmd, tmp));
	else
		return (parse_simple_command(cmd, tmp));
}
