/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 17:30:41 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char		*ext_unit(char *src, size_t start, size_t end);
int			quote_wrapper(t_gen_word *gw, char *value, size_t *i);
int			doller_literal_wrapper(t_gen_word *gw, char *value,
				size_t value_len, size_t *i);
int			init_gen_word_data(t_word **word, t_gen_word *gw, char *value,
				size_t *i);
size_t		quote_close_place(char quote, char *value, size_t i);

static int	handle_quoted_word(t_word *word, t_gen_word *gw, char *value,
		size_t *i)
{
	(void)word;
	if (quote_wrapper(gw, value, i) < 0)
		return (-1);
	return (0);
}

static int	handle_literal_word(t_word *word, t_gen_word *gw, char *value,
		size_t value_len, size_t *i)
{
	doller_literal_wrapper(gw, value, value_len, i);
	word->word = ext_unit(value, gw->start, *i);
	if (!word->word)
		return (-1);
	return (0);
}

t_word	*loop_wrapper(char *value, size_t value_len, size_t *i)
{
	t_word		*word;
	t_gen_word	gw;

	if (init_gen_word_data(&word, &gw, value, i) < 0)
		return (NULL);
	gw.start = *i;
	if (gw.quote != '\0')
		gw.close_place = quote_close_place(gw.quote, value, ++(*i));
	if (gw.quote != '\0' && gw.close_place > 0)
	{
		if (handle_quoted_word(word, &gw, value, i) < 0)
			return (xfree(word), NULL);
	}
	else if (handle_literal_word(word, &gw, value, value_len, i) < 0)
		return (xfree(word), NULL);
	return (word);
}
