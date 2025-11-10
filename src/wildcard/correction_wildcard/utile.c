
#include "minishell.h"

static size_t	count_digits(int n)
{
	size_t	count;

	if (n == 0)
		return (1);
	count = 0;
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

void	*xmalloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

char	**ft_split(const char *str, char c)
{
	size_t	len;
	size_t	count;
	size_t	idx;
	size_t	start;
	size_t	seg_len;
	char	*part;
	char	**res;

	if (!str)
		return (NULL);
	len = strlen(str);
	count = 1;
	for (size_t i = 0; i < len; i++)
		if (str[i] == c)
			count++;
	res = calloc(count + 2, sizeof(char *));
	if (!res)
		return (NULL);
	idx = 0;
	start = 0;
	for (size_t i = 0; i <= len; i++)
	{
		if (str[i] == c || str[i] == '\0')
		{
			seg_len = i - start;
			part = malloc(seg_len + 1);
			if (!part)
				return (NULL);
			memcpy(part, str + start, seg_len);
			part[seg_len] = '\0';
			res[idx++] = part;
			start = i + 1;
		}
	}
	res[idx] = NULL;
	return (res);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;
	long	num;

	num = n;
	len = count_digits(n);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	if (num == 0)
		str[0] = '0';
	while (num > 0)
	{
		len--;
		str[len] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}
