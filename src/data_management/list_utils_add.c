#include "../../includes/minishell.h"

// Add redirection to the end of the list
void	redir_add_back(t_redir **head, t_redir *new)
{
	t_redir	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = redir_last(*head);
	last->next = new;
}

// Add word to the end of the list
void	word_add_back(t_word **head, t_word *new)
{
	t_word	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = word_last(*head);
	last->next = new;
}

// Add token to the end of the list
void	token_add_back(t_token **head, t_token *new)
{
	t_token	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = token_last(*head);
	last->next = new;
	new->prev = last;
}
