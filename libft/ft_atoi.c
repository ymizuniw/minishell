/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:36:51 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/20 16:34:02 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int			overflow;
	long long	result;

	result = ft_atoll(nptr, &overflow);
	if (overflow)
		return (0);
	if (result > INT_MAX)
		return (0);
	if (result < INT_MIN)
		return (0);
	return ((int)result);
}
