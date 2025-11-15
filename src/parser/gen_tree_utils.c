/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_tree_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 22:31:44 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	build_tree(t_ast **parent, t_token **cur_token);

t_ast	*handle_operator(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*right;

	node = swap_with_parent(parent, cur_token);
	*parent = node;
	right = NULL;
	build_tree(&right, cur_token);
	node->right = right;
	return (node);
}
