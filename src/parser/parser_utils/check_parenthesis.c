#include "../../../includes/minishell.h"

// check if parenthesis arrangement is correct.
int	check_parenthesis(t_token *token)
{
	t_token	*cur;
	int		check;

	if (!token)
		return (-1);
	cur = token->prev;
	while (cur != NULL)
	{
		if (cur->type == TK_LPAREN)
			return (1);
		if (cur->type == TK_RPAREN)
		{
			check = check_parenthesis(cur);
			if (check < 0)
				return (-1);
		}
		cur = cur->prev;
	}
	return (-1);
}
