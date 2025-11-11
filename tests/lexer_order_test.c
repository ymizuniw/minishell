#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_AND,
	T_OR,
	T_IN,
	T_OUT,
	T_APPEND,
	T_HEREDOC,
	T_LPAREN,
	T_RPAREN,
	T_EOF
}					t_token_type;

typedef struct s_token
{
	char			*value;
	char			*word;
	struct s_token	*next;
	struct s_token	*prev;
	t_token_type	type;
	int				in_squote;
	int				in_dquote;
}					t_token;

// Simplified prepend_tokens to demonstrate order
void	prepend_token(t_token **head, const char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->value = strdup(value);
	new->word = strdup(value);
	new->type = T_WORD;
	new->in_squote = 0;
	new->in_dquote = 0;
	new->prev = NULL;
	if (*head == NULL)
	{
		new->next = NULL;
		*head = new;
	}
	else
	{
		// Insert at head (LIFO)
		new->next = *head;
		(*head)->prev = new;
		*head = new;
	}
}

void	print_forward(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	printf("Forward (head->next->...):\n");
	while (cur)
	{
		printf("  Token %d: '%s'\n", i, cur->value);
		cur = cur->next;
		i++;
	}
}

void	print_backward(t_token *head)
{
	t_token	*cur;
	int		i;

	cur = head;
	i = 0;
	// Find tail
	while (cur && cur->next)
		cur = cur->next;
	printf("\nBackward (tail->prev->...):\n");
	while (cur)
	{
		printf("  Token %d: '%s'\n", i, cur->value);
		cur = cur->prev;
		i++;
	}
}

int	main(void)
{
	t_token	*head;

	head = NULL;
	printf("=== Simulating: echo hello world ===\n");
	printf("Adding tokens in order: echo, hello, world\n\n");
	// Simulate lexer adding tokens in order encountered
	prepend_token(&head, "echo");
	prepend_token(&head, "hello");
	prepend_token(&head, "world");
	print_forward(head);
	print_backward(head);
	printf("\n=== Expected behavior ===\n");
	printf("If parser starts from tail and goes backward,\n");
	printf("it should process: world -> hello -> echo\n");
	printf("But we want: echo -> hello -> world\n");
	return (0);
}
