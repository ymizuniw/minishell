/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_token_type.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:40:35 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:40:35 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*
** Set token's type by calling get_token_type
*/
void	set_token_type(t_token *token, char *input, size_t *tmp_idx)
{
	token->type = get_token_type(input, tmp_idx);
}
