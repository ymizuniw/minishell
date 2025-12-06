/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted_word_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:41:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:42:32 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

// t_word			*create_word_node(char *content, t_word_type type,
// 					bool expand_doller);
// t_word			*append_node(t_word *head, t_word *new);

t_word	*handle_quoted_word(const char *input, size_t *idx, char quote)
{
	size_t	start;
	size_t	len;
	t_word	*words;

	start = *idx + 1;
	len = 0;
	while (input[start + len] && input[start + len] != quote)
		len++;
	if (input[start + len] != quote)
		return (NULL);
	*idx = start + len + 1;
	words = process_quoted_content(&input[start], len, (quote == '"'));
	return (words);
}
