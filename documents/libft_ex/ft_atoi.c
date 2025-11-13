/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 22:21:38 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/06/26 17:26:59 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	res;

	sign = 1;
	res = 0;
	while (*nptr == 32 || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while ('0' <= *nptr && '9' >= *nptr)
	{
		res = 10 * res + (*nptr - '0');
		nptr++;
	}
	return (res * (sign));
}

// int	main(void)
// {
// 	const char	str[] = "12345";

// 	int	result = ft_atoi(str);
// 	printf("%d", result);
// 	return (0);
// }
