#include "libft.h"

static int	parse_sign(const char **s)
{
	int	sign;

	sign = 1;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	return (sign);
}

static int	check_overflow(long long n, int digit, int sign)
{
	if (sign == 1 && n > (LLONG_MAX - digit) / 10)
		return (1);
	if (sign == -1 && n > (-(LLONG_MIN + digit)) / 10)
		return (1);
	return (0);
}

long long	ft_atoll(const char *s, int *overflow)
{
	long long	n;
	int			sign;
	int			digit;

	*overflow = 0;
	while (*s == 32 || (*s >= 9 && *s <= 13))
		s++;
	n = 0;
	sign = parse_sign(&s);
	while (*s)
	{
		if (!ft_isdigit(*s))
		{
			*overflow = 1;
			return (0);
		}
		digit = *s - '0';
		if (check_overflow(n, digit, sign))
		{
			*overflow = 1;
			return (0);
		}
		n = n * 10 + digit;
		s++;
	}
	return (n * sign);
}
