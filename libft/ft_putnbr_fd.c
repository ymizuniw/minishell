/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 05:15:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 16:50:36 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	zero_min_case(int n, int fd)
{
	if (n == 0)
		write(fd, "0", 1);
	else if (n == -2147483648)
		write(fd, "-2147483648", 11);
}

static int	sign_handler(int n)
{
	if (n < 0)
		return (-1);
	else
		return (1);
}

void	ft_putnbr_fd(int n, int fd)
{
	int	sign;

	if (n == 0 || n == -2147483648)
	{
		zero_min_case(n, fd);
		return ;
	}
	sign = sign_handler(n);
	if (sign == -1)
		write(fd, "-", 1);
	n = n * sign;
	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	ft_putchar_fd(n % 10 + '0', fd);
}
