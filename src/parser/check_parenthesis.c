#include "../../includes/minishell.h"

// Check if parentheses are empty: () is invalid
static int	check_empty_parenthesis(t_token *lparen)
{
	t_token	*next;

	if (!lparen || lparen->type != TK_LPAREN)
		return (1);
	next = lparen->next;
	if (next && next->type == TK_RPAREN)
		return (0);
	return (1);
}

int	check_parenthesis(t_token *token)
{
	t_token	*cur;
	int		check;

	if (!token)
		return (-1);
	cur = token->prev;
	if (cur->type == TK_LPAREN)
	{
		// Check for empty parentheses
		if (!check_empty_parenthesis(cur))
			return (-1);
		return (-1);
	}
	while (cur != NULL)
	{
		if (cur->type == TK_LPAREN)
		{
			// Check for empty parentheses
			if (!check_empty_parenthesis(cur))
				return (-1);
			return (1);
		}
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
	cur = token_last(token_list);
	while (cur && cur->type != TK_HEAD)
	{
		if (cur->type == TK_LPAREN)
		{
			// Check for empty parentheses: when going backwards,
			// if we see LPAREN and prev is RPAREN, it means () pattern
			if (cur->prev && cur->prev->type == TK_RPAREN)
			{
				return (-1);
			}
			balance++;
		}
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
