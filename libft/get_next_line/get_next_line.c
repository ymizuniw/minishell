/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 01:59:07 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 19:05:22 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*while_loop(int fd, char *remainder, char *buffer)
{
	ssize_t	bytes_read;
	char	*tmp;

	while (!remainder || !ft_strchr(remainder, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(remainder);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(remainder, buffer);
		if (!tmp)
		{
			free(remainder);
			return (NULL);
		}
		free(remainder);
		remainder = tmp;
	}
	return (remainder);
}

static char	*read_and_accumulate(int fd, char *remainder)
{
	char	*buffer;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		free(remainder);
		return (NULL);
	}
	remainder = while_loop(fd, remainder, buffer);
	free(buffer);
	return (remainder);
}

static char	*extract_line(const char *str)
{
	int		i;
	char	*line;

	if (!str || str[0] == '\0')
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	ft_memcpy(line, str, i);
	line[i] = '\0';
	return (line);
}

static char	*save_remainder(char *str)
{
	int		i;
	int		j;
	char	*remainder;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	if (str[i] == '\0')
		return (NULL);
	remainder = malloc(ft_strlen(str + i) + 1);
	if (!remainder)
		return (NULL);
	while (str[i])
		remainder[j++] = str[i++];
	remainder[j] = '\0';
	return (remainder);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*tmp;
	static char	*remainder;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	remainder = read_and_accumulate(fd, remainder);
	if (!remainder || remainder[0] == '\0')
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	line = extract_line(remainder);
	if (!line)
	{
		free(remainder);
		remainder = NULL;
		return (NULL);
	}
	tmp = save_remainder(remainder);
	free(remainder);
	remainder = tmp;
	return (line);
}
