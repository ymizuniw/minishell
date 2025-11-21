/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mkstmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 21:04:21 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	ft_mkstmpfd(void)
{
	int	fd;

	fd = open("/tmp", O_EXCL | __O_TMPFILE | __O_DIRECTORY | O_RDWR, 0600);
	if (fd >= 0)
		return (fd);
	return (-1);
}
