/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:39:08 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:39:09 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_word	*append_node(t_word *head, t_word *new);
t_word	*loop_wrapper(char *value, size_t value_len, size_t *i);

t_word	*gen_word(char *value, size_t value_len, size_t *addition)
{
	size_t	i;
	t_word	*head;
	t_word	*word;
	t_word	*new_head;

	head = NULL;
	word = NULL;
	i = 0;
	while (i < value_len)
	{
		word = loop_wrapper(value, value_len, &i);
		if (word == NULL)
			return (free_word_list(head), NULL);
		new_head = append_node(head, word);
		if (!new_head && head != NULL)
			return (xfree(word), free_word_list(head), NULL);
		head = new_head;
	}
	*addition = i;
	return (head);
}
