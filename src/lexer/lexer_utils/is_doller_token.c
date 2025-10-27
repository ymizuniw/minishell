#include "../../../includes/minishell.h"

/*
name	:is_doller_token
args	:const char *input, size_t *current_idx_p
purpose: find patterns that will be printed as one '$', that are "$", '$',
	$"var", $'var', $<space>, $<eof>
return : 1 if it is doller token, 0 in other cases.
*/
bool	is_doller_token(const char *p)
{
	if (p[0]!='$')
		return (false);
	if (p[1] == '\0')
		return (true);
	if (isspace((unsigned char)p[1]))
		return (true);
	if (p[1] == '\'' || p[1] == '\"')
		return (true);
	return (false);
}
