#include "../../../includes/minishell.h"

// return either of double/single quotation, or '\0'.
unsigned char	is_quote(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	if (uc == '\"')
		return ('\"');
	else if (uc == '\'')
		return ('\'');
	else
		return ('\0');
}
