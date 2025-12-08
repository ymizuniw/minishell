/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/08 13:51:37 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_word			*append_node(t_word *head, t_word *new);

static t_word	*dispatch_word(char *value, size_t value_len, size_t *i)
{
	char	c;
	t_word	*word;

	c = value[*i];
	if (is_quote(c))
	{
		word = handle_quoted_word(value, i, c);
		word->quoted = true;
	}
	else if (c == '$')
		word = handle_doller_word(value, value_len, i);
	else
		word = handle_unquoted_word(value, value_len, i);
	return (word);
}

t_word	*gen_word(char *value, size_t value_len, size_t *addition)
{
	size_t	i;
	t_word	*head;
	t_word	*word;

	head = NULL;
	i = 0;
	while (i < value_len)
	{
		word = dispatch_word(value, value_len, &i);
		if (!word)
		{
			free_word_list(head);
			return (NULL);
		}
		head = append_node(head, word);
	}
	*addition = i;
	return (head);
}
