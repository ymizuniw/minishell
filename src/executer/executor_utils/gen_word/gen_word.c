/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/04 03:25:15 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_word	*append_node(t_word *head, t_word *new);

t_word	*gen_word(char *value, size_t value_len, size_t *addition)
{
	size_t	i;
	t_word	*head;
	t_word	*word;
	char	c;

	head = NULL;
	i = 0;
	while (i < value_len)
	{
		c = value[i];
		if (is_quote(c))//set t_word: quoted flag.
		{
			word->quoted=true;
			word = handle_quoted_word(value, &i, c);
		}
		else if (c == '$')
			word = handle_doller_word(value, value_len, &i);
		else
			word = handle_unquoted_word(value, value_len, &i);
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
