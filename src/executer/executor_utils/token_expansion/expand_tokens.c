/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:29:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 18:53:34 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static bool	validate_token(t_token *token)
{
	return (token && token->value);
}

static t_word	*make_word_list(t_token *token)
{
	size_t	value_len;
	size_t	additional;

	value_len = ft_strlen(token->value);
	additional = 0;
	return (gen_word(token->value, value_len, &additional));
}

static char	*expand_word_list(t_word *word_list, t_shell *shell, bool *quoted)
{
	return (expand_plain_word(word_list, shell, quoted));
}

static char	**finalize_expansion(char *expanded, bool quoted)
{
	char	**res;

	if (quoted)
	{
		res = xcalloc(sizeof(char *) * 2);
		if (!res)
			return (NULL);
		res[0] = expanded;
		res[1] = NULL;
		return (res);
	}
	res = split_by_ifs(expanded);
	free(expanded);
	return (res);
}

char	**expand_token(t_token *token, t_shell *shell)
{
	bool	quoted;
	t_word	*word_list;
	char	*expanded;

	quoted = false;
	if (!validate_token(token))
		return (NULL);
	word_list = make_word_list(token);
	if (!word_list)
		return (NULL);
	expanded = expand_word_list(word_list, shell, &quoted);
	free_word_list(word_list);
	if (!expanded)
		return (NULL);
	return (finalize_expansion(expanded, quoted));
}
