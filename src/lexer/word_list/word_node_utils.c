#include "../../../includes/minishell.h"

t_word	*create_word_node(char *content, t_word_type type, bool expand_doller,
		bool expand_wildcard)
{
	t_word	*word;

	word = (t_word *)xcalloc(sizeof(t_word));
	if (!word)
		return (NULL);
	word->word = content;
	word->type = type;
	word->to_expand_doller = expand_doller;
	word->to_expand_wildcard = expand_wildcard;
	word->next = NULL;
	return (word);
}

t_word	*append_node(t_word *head, t_word *new)
{
	t_word	*current;

	if (!head)
		return (new);
	current = head;
	while (current->next)
		current = current->next;
	current->next = new;
	return (head);
}
