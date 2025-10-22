#include "../../../includes/minishell.h"

// append_token should add front to parse from last token.
void	append_tokens(t_token *head, t_token *new)
{
	if (!head || !new)
		return ;
	new->next = head->next;
	if (head->next)
		head->next->prev = new;
	head->next = new;
	new->prev = head;
	head->size++;
}
