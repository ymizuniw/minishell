#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*ret;
	char		*nl;
	ssize_t		rb;
	size_t		ret_len;
	size_t		chunk;
	char		*new;
	size_t		rem;
	size_t		tmp_len;

	tmp_len = 0;
	ret = NULL;
	ret_len = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (buffer[0])
		{
			nl = ft_strchr(buffer, '\n');
			if (nl)
			{
				chunk = (size_t)(nl - buffer) + 1;
				if (ret)
					tmp_len = ft_strlen(ret) + 1;
				else
					tmp_len = 0;
				new = ft_realloc(ret, tmp_len, ret_len + chunk + 1);
				if (!new)
					return (free(ret), NULL);
				ft_memcpy(new + ret_len, buffer, chunk);
				new[ret_len + chunk] = '\0';
				ret = new;
				ret_len += chunk;
				rem = ft_strlen(buffer + chunk);
				ft_memmove(buffer, buffer + chunk, rem + 1);
				return (ret);
			}
			else
			{
				chunk = ft_strlen(buffer);
				new = ft_realloc(ret, ret ? ft_strlen(ret) + 1 : 0, ret_len
						+ chunk + 1);
				if (!new)
					return (free(ret), NULL);
				ft_memcpy(new + ret_len, buffer, chunk);
				ret = new;
				ret_len += chunk;
				buffer[0] = '\0';
			}
		}
		rb = read(fd, buffer, BUFFER_SIZE);
		if (rb <= 0)
		{
			if (ret_len > 0)
				return (ret);
			free(ret);
			return (NULL);
		}
		buffer[rb] = '\0';
	}
}
