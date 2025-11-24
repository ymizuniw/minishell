/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_t.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:14 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/24 23:33:37 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// these may well have bzero?
t_ast	*alloc_node(void)
{
	t_ast *p = (t_ast *)xmalloc(sizeof(t_ast));
	fprintf(stderr, "[ALLOC NODE] %p\n", p);
	// return ((t_ast *)xmalloc(sizeof(t_ast)));
	return (p);
}

t_cmd	*alloc_cmd(void)
{
	t_cmd *p = (t_cmd *)xmalloc(sizeof(t_cmd));
	fprintf(stderr, "[ALLOC CMD] %p\n", p);
	// return ((t_ast *)xmalloc(sizeof(t_ast)));
	return (p);
	// return ((t_cmd *)xmalloc(sizeof(t_cmd)));
}

t_token	*alloc_token(void)
{

	t_token *tok = xmalloc(sizeof(t_token));
    fprintf(stderr, "[ALLOC TOKEN] %p\n", tok);
    return tok;
	// return ((t_token *)xmalloc(sizeof(t_token)));
}

t_redir	*alloc_redir(void)
{

	t_redir *p = (t_redir *)xmalloc(sizeof(t_redir));
	fprintf(stderr, "[ALLOC REDIR] %p\n", p);
	// return ((t_ast *)xmalloc(sizeof(t_ast)));
	return (p);
	// return ((t_redir *)xmalloc(sizeof(t_redir)));
}
