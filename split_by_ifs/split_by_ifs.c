
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *xmalloc(size_t size)
{
	void *p;

	p = (void *)malloc(size);
	if (p == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	return (p);
}

size_t ft_strlen(const char *s)
{
	size_t i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t i;

	i = 0;
	if (size > 0)
	{
		while (i < size - 1 && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

char *ft_strdup(const char *s)
{
	int len;
	char *dup;

	len = ft_strlen(s);
	dup = xmalloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_strlcpy(dup, s, len + 1);
	return (dup);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t i;
	char *str;
	size_t l;

	i = 0;
	if (s == NULL)
		return (NULL);
	l = ft_strlen(s);
	if (start >= l)
		return (ft_strdup(""));
	if (len > l - start)
		len = l - start;
	str = malloc(len + 1);
	if (str == NULL)
		return (NULL);
	while (i < len && s[start + i])
	{
		str[i] = s[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}

// delete after ↑↑

static int ft_isifs(char s)
{
	return (s == ' ' || s == '\t' || s == '\n');
}

static size_t count_word(char *str)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break;
		while (str[i] && !ft_isifs(str[i]))
			i++;
		j++;
	}
	return (j);
}

static int extract_word(char **result, size_t *idx, char *str, size_t *i)
{
	size_t start;

	start = *i;
	while (str[*i] && !ft_isifs(str[*i]))
		(*i)++;
	result[*idx] = ft_substr(str, start, *i - start);
	if (!result[*idx])
	{
		while (*idx > 0)
		{
			(*idx)--;
			free(result[*idx]);
		}
		free(result);
		return (0);
	}
	(*idx)++;
	return (1);
}

char **split_by_ifs(char *str)
{
	size_t i;
	size_t word;
	char **result;

	i = 0;
	word = count_word(str);
	result = xmalloc(sizeof(char *) * (word + 1));
	if (!result)
		return (NULL);
	size_t idx = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break;
		if (!extract_word(result, &idx, str, &i))
			return (NULL);
	}
	result[idx] = NULL;
	return (result);
}

int main(void)
{
	char *str;
	char **res;
	int i = 0;

	str = "This\t function \nis\t\t verry  good\n";
	res = split_by_ifs(str);
	while (res[i])
	{
		printf("%s\n", res[i]);
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}
