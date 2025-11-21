/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mkstmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 22:37:12 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	generate_random_template(char *filename)
{
	int		fd;
	ssize_t	rb;

	static const char safe_charset[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
											"abcdefghijklmnopqrstuvwxyz"
											"0123456789-_";
	static const char tmp_dir[5] = "/tmp/";
	unsigned char raw[16];

	ft_memset(raw, 0, 16);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (-1);
	rb = read(fd, raw, 16);
	close(fd);
	if (rb < 0)
		return (-1);
	size_t i = 0;
	ft_memcpy(filename, tmp_dir, 5);
	while (i<16)
	{
		filename[5+i] = safe_charset[raw[i] & 0x3F];
		i++;
	}
	return (1);
}

// 	// unsigned char filename[256];
// int	ft_mkstmpfd(char **filename)
// {
// 	int	fd;

// 	ft_memset(filename, 0, 256);
// 	// fd = open("/tmp", O_EXCL | __O_TMPFILE | __O_DIRECTORY | O_RDWR, 0600);
// 	fd = open(*filename, O_RDWR, 0600);
// 	if (fd >= 0)
// 		return (fd);
// 	return (-1);
// }

// 2^6.
// 00[00 0000]
// 0x3F