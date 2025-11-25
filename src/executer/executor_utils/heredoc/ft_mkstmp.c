/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mkstmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/26 02:33:52 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	generate_random_template(char *filename)
{
	int					fd;
	ssize_t				rb;
	static const char	tmp_dir[6] = "/tmp/";
	unsigned char		raw[16];
	size_t				i;

	ft_memset(raw, 0, 16);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (-1);
	rb = read(fd, raw, 16);
	close(fd);
	if (rb < 0)
		return (-1);
	i = 0;
	ft_memcpy(filename, tmp_dir, 5);
	while (i < 16)
	{
		filename[5 + i] = SAFE_CHARSET[raw[i] & 0x3F];
		i++;
	}
	return (1);
}
