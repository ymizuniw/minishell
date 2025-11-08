
#include "minishell_libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	int		i;

	i = 0;
	str = malloc(n + 1);
	if (!str)
		return (NULL);
	while (n > i)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	main(void)
{
	char	*str;
	char	*res;

	str = "hello";
	res = ft_strndup(str, 3);
	printf("%s\n", res);
	return (0);
}
