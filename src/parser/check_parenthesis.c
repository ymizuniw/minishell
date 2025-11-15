#include "../../includes/minishell.h"

int	check_empty_parenthesis(t_token *lparen);
int	process_lparen(t_token *cur);
int	process_rparen(t_token *cur);
int	check_backward_balance(t_token *cur, int *balance);

int	check_parenthesis(t_token *token)
{
	t_token	*cur;
	int		result;

	if (!token)
		return (-1);
	cur = token->prev;
	if (cur->type == TK_LPAREN)
		return (process_lparen(cur));
	while (cur != NULL)
	{
		if (cur->type == TK_LPAREN)
			return (process_lparen(cur));
		if (cur->type == TK_RPAREN)
		{
			result = process_rparen(cur);
			if (result < 0)
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
		if (check_backward_balance(cur, &balance) < 0)
			return (-1);
		cur = cur->prev;
	}
	if (balance > 0)
		return (1);
	return (0);
}
