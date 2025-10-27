#include "../../../includes/minishell.h"

// head is a dummy head. append new at the tail (FIFO)
void	append_tokens(t_token *head, t_token *new)
{
	if (!head || !new)
		return;
	new->next = NULL;
	if (!head->next)
	{
		head->next = new;
		new->prev = head;
		head->size++;
		return;
	}
	t_token *cur = head->next;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
	new->prev = cur;
	head->size++;
}
