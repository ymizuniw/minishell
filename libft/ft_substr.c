
#include "minishell_libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	l;

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

