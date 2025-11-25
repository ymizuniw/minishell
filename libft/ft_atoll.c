/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:05 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/26 01:09:59 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	parse_sign(const char **s)
{
	int	sign;

	sign = 1;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	return (sign);
}

long long	ft_atoll(const char *s, int *overflow)
{
	long long	res;
	int			sign;

	while (ft_isspace(*s))
		s++;
	res = 0;
	sign = parse_sign(&s);
	while (*s)
	{
		if (!ft_isdigit(*s))
		{
			*overflow = 1;
			return (0);
		}
		res = 10 * res + (*s - '0');
		if ((sign == -1 && (-res) < INT_MIN) || (sign == 1 && res > INT_MAX))
		{
			*overflow = 1;
			return (0);
		}
		s++;
	}
	return (res*sign);
}
