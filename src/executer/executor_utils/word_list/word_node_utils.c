/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_node_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:22 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/05 18:17:26 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_word	*create_word_node(char *content, t_word_type type, bool expand_doller)
{
	t_word	*word;

	word = (t_word *)xcalloc(sizeof(t_word));
	if (!word)
		return (NULL);
	word->word = content;
	word->type = type;
	word->to_expand_doller = expand_doller;
	word->next = NULL;
	return (word);
}

t_word	*append_node(t_word *head, t_word *new)
{
	if (!head)
		return (new);
	ft_lst_add_back((void **)&head, new, offsetof(t_word, next));
	return (head);
}
