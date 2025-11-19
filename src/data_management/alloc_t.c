/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:14 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/19 19:27:06 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// these may well have bzero?
t_ast	*alloc_node(void)
{
	return ((t_ast *)xmalloc(sizeof(t_ast)));
}

t_cmd	*alloc_cmd(void)
{
	return ((t_cmd *)xmalloc(sizeof(t_cmd)));
}

t_token	*alloc_token(void)
{
	return ((t_token *)xmalloc(sizeof(t_token)));
}

t_redir	*alloc_redir(void)
{
	return ((t_redir *)xmalloc(sizeof(t_redir)));
}

t_result	*alloc_result(void)
{
	return ((t_result *)xmalloc(sizeof(t_result)));
}
