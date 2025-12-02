/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:14 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:16:26 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_ast	*alloc_node(void)
{
	t_ast	*p;

	p = (t_ast *)xmalloc(sizeof(t_ast));
	return (p);
}

t_cmd	*alloc_cmd(void)
{
	t_cmd	*p;

	p = (t_cmd *)xmalloc(sizeof(t_cmd));
	return (p);
}

t_token	*alloc_token(void)
{
	t_token	*tok;

	tok = xmalloc(sizeof(t_token));
	return (tok);
}

t_redir	*alloc_redir(void)
{
	t_redir	*p;

	p = (t_redir *)xmalloc(sizeof(t_redir));
	return (p);
}
