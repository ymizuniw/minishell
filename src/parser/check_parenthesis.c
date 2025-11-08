#include "../../includes/minishell.h"

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

int	check_parenthesis_balance(t_token *token_list)
{
	t_token	*cur;
	int		balance;

	if (!token_list)
		return (0);
	balance = 0;
	cur = token_list;
	while (cur && cur->next)
		cur = cur->next;
	while (cur && cur->type != TK_HEAD)
	{
		if (cur->type == TK_LPAREN)
			balance++;
		else if (cur->type == TK_RPAREN)
		{
			balance--;
			if (balance < 0)
				return (-1);
		}
		cur = cur->prev;
	}
	if (balance > 0)
		return (1);
	return (0);
}
