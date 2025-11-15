/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_cat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:40:39 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 18:40:45 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	handle_quotation(t_word_cat *wc, size_t word_len, size_t *consumed_len,
		char quote_open)
{
	size_t	add_len;
	char	*quote_close;

	quote_close = NULL;
	if (wc->input[*wc->idx + 1])
		quote_close = ft_strchr(&wc->input[*wc->idx + 1], quote_open);
	if (!quote_close)
		return (0);
	add_len = quote_close - &wc->input[*wc->idx] - 1;
	*wc->word = ft_realloc(*wc->word, sizeof(char) * (word_len + 1),
			sizeof(char) * (word_len + add_len + 1));
	if (!*wc->word)
		return (-1);
	ft_memcpy(*wc->word + word_len, &wc->input[*wc->idx + 1], add_len);
	(*wc->word)[add_len] = '\0';
	*wc->idx += add_len + 2;
	*consumed_len = add_len;
	return (1);
}

int	handle_plain(t_word_cat *wc, size_t *word_len)
{
	size_t		add_len;
	const char	*tmp_ptr;

	add_len = 0;
	tmp_ptr = &wc->input[*wc->idx];
	while (*wc->idx < wc->input_len
		&& !ft_isspace((unsigned char)wc->input[*wc->idx])
		&& is_meta_char(wc->input[*wc->idx]) == MT_OTHER)
		(*wc->idx)++;
	add_len = &wc->input[*wc->idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*wc->word = ft_realloc(*wc->word, sizeof(char) * (*word_len + 1),
			sizeof(char) * (*word_len + (add_len) + 1));
	if (!*wc->word)
		return (-1);
	ft_memcpy(*wc->word + *word_len, tmp_ptr, add_len);
	(*wc->word)[*word_len + add_len] = '\0';
	*word_len += add_len;
	return (1);
}

int	extract_word_loop(t_word_cat *wc, char *quote)
{
	while (*wc->idx < wc->input_len)
	{
		if (!*quote && is_quote(wc->input[*wc->idx]))
			*quote = wc->input[*wc->idx];
		else if (*quote && wc->input[*wc->idx] == *quote)
			*quote = 0;
		else if (!*quote && (ft_isspace((unsigned char)wc->input[*wc->idx])
				|| is_meta_char(wc->input[*wc->idx]) != MT_OTHER))
			break ;
		(*wc->idx)++;
	}
	return (1);
}

size_t	word_cat(t_word_cat *wc, size_t word_len)
{
	char	quote;
	size_t	start;

	start = *wc->idx;
	quote = 0;
	extract_word_loop(wc, &quote);
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (0);
	}
	*wc->word = ft_realloc(*wc->word, sizeof(char) * (word_len + 1),
			sizeof(char) * (word_len + (*wc->idx - start) + 1));
	if (!*wc->word)
		return (0);
	ft_memcpy(*wc->word + word_len, &wc->input[start], *wc->idx - start);
	(*wc->word)[word_len + (*wc->idx - start)] = '\0';
	return (1);
}
