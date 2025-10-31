#include "../../../includes/minishell.h"

// head is a dummy head. append new at the tail (FIFO)
void	prepend_tokens(t_token *head, t_token *new)
{
	t_token	*cur_first;

	cur_first = NULL;
	if (!head || !new)
		return ;
	if (head->next)
		cur_first = head->next;
	if (cur_first != NULL)
		cur_first->prev = new;
	head->next = new;
	new->next = cur_first;
	new->prev = head;
	head->size++;
}
