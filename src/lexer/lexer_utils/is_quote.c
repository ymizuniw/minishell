#include "../../../includes/minishell.h"

// Check if character is a quote and return the quote type
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
