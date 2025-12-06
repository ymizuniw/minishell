/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:10:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 19:21:43 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	expand_exit_status(char **res, t_shell *shell, size_t *i)
{
	char	*s;
	size_t	res_len;
	size_t	s_len;

	s = ft_itoa(shell->last_exit_status);
	if (!s)
	{
		xfree(*res);
		return (false);
	}
	res_len = 0;
	s_len = 0;
	if (*res)
		res_len = ft_strlen(*res);
	if (s)
		s_len = ft_strlen(s);
	if (!join_value(res, s, res_len, s_len))
	{
		xfree(s);
		xfree(*res);
		return (false);
	}
	xfree(s);
	*i += 2;
	return (true);
}
