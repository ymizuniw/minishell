#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int			overflow;
	long long	result;

	result = ft_atoll(nptr, &overflow);
	if (overflow)
		return (0);
	if (result > INT_MAX)
		return (0);
	if (result < INT_MIN)
		return (0);
	return ((int)result);
}
