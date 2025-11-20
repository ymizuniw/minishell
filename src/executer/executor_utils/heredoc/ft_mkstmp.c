/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mkstmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/20 19:05:34 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	ft_mkstmpfd(char *template, char **filename)
{
	int	fd;

	(void)template;
	(void)filename;
	fd = open("/tmp", O_EXCL | __O_TMPFILE, 0600);
	if (fd >= 0)
		return (fd);
	return (-1);
}
