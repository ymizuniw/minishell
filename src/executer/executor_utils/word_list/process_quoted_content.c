/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_quoted_content.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:36:07 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:43:22 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static t_word	*append_or_free(t_word *head, t_word *new_node);
static t_word	*generate_empty_literal(void);
static int		process_loop(const char *content, size_t len,
					bool is_double_quote, t_word **head);

static t_word	*append_or_free(t_word *head, t_word *new_node)
{
	if (!new_node)
	{
		free_word_list(head);
		return (NULL);
	}
	return (append_node(head, new_node));
}

static t_word	*generate_empty_literal(void)
{
	t_word	*node;

	node = create_word_node(ft_strdup(""), WD_LITERAL, false);
	if (!node || !node->word)
	{
		free_word_list(node);
		return (NULL);
	}
	return (node);
}

static int	process_loop(const char *content, size_t len, bool is_double_quote,
		t_word **head)
{
	size_t	i;
	t_word	*segment;

	i = 0;
	while (i < len)
	{
		segment = process_content_segment(content, &i, len, is_double_quote);
		*head = append_or_free(*head, segment);
		if (!*head)
			return (-1);
	}
	return (0);
}

t_word	*process_quoted_content(const char *content, size_t len,
		bool is_double_quote)
{
	t_word	*head;

	head = NULL;
	if (process_loop(content, len, is_double_quote, &head) < 0)
		return (NULL);
	if (!head)
	{
		head = generate_empty_literal();
		if (!head)
			return (NULL);
	}
	return (head);
}
