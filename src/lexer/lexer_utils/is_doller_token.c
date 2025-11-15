/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_doller_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:25 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:39:31 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

bool	is_doller_token(const char *p)
{
	if (p[0] != '$')
		return (false);
	if (p[1] == '\0')
		return (true);
	if (ft_isspace((unsigned char)p[1]))
		return (true);
	return (false);
}
