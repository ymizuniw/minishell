#include "../../../includes/minishell.h"

bool	has_unclosed_quotes(const char *line)
{
	bool	in_sq;
	bool	in_dq;

	in_sq = false;
	in_dq = false;
	if (!line)
		return (false);
	for (size_t i = 0; line[i]; ++i)
	{
		if (line[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (line[i] == '"' && !in_sq)
			in_dq = !in_dq;
	}
	return (in_sq || in_dq);
}
