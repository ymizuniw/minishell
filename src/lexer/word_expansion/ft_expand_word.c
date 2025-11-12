/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/12 10:27:17 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	process_word_expansion(t_word *word, char ***res, size_t *i,
		t_expand_ctx *ctx)
{
	if (word->to_expand_doller)
	{
		if (!add_dollar_expansion(word, *res, *i, ctx->shell))
			return (0);
		(*i)++;
	}
	else if (word->to_expand_wildcard)
	{
		if (!expand_wildcard_to_result(word, res, i, &ctx->word_list_count))
			return (0);
	}
	else
	{
		if (!add_plain_word(word, *res, *i))
			return (0);
		(*i)++;
	}
	return (1);
}

char	**ft_expand_word(t_word *wd_list, t_shell *shell)
{
	t_expand_ctx	ctx;
	char			**res;
	size_t			i;
	t_word			*word;

	ctx.shell = shell;
	ctx.word_list_count = count_word_list(wd_list);
	res = (char **)xcalloc(sizeof(char *) * (ctx.word_list_count + 1));
	if (!res)
		return (NULL);
	i = 0;
	word = wd_list;
	while (i < ctx.word_list_count && word)
	{
		if (!process_word_expansion(word, &res, &i, &ctx))
			return (free_double_array(res), NULL);
		word = word->next;
	}
	res[i] = NULL;
	return (res);
}
