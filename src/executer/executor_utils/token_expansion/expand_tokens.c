/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:29:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 18:01:55 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char	**expand_token(t_token *token, t_shell *shell)
{
	size_t	value_len;
	size_t	additional;
	t_word	*word_list;
	char	**res;
	size_t	count;

	if (!token || !token->value)
		return (NULL);
	value_len = ft_strlen(token->value);
	additional = 0;
	word_list = gen_word(token->value, value_len, &additional);
	if (!word_list)
		return (NULL);
	if (has_wildcard_to_expand(word_list))
	{
		count = 0;
		res = expand_token_with_wildcard(word_list, shell, &count);
		free_word_list(word_list);
		return (res);
	}
	res = (char **)xcalloc(sizeof(char *) * 2);
	if (!res)
	{
		free_word_list(word_list);
		return (NULL);
	}
	res[0] = expand_plain_word(word_list, shell);
	res[1] = NULL;
	free_word_list(word_list);
	return (res);
}

char	**expand_tokens(t_token **tokens, size_t count, t_shell *shell)
{
	char	**argv;
	size_t	argc;
	char	**expanded;

	argv = NULL;
	argc = 0;
	for (size_t i = 0; i < count; i++)
	{
		expanded = expand_token(tokens[i], shell);
		if (!expanded)
			return (free_double_array(argv), NULL);
		for (size_t j = 0; expanded[j]; j++)
		{
			argv = append_argv(argv, &argc, expanded[j]);
		}
		free_double_array(expanded);
	}
	return (argv);
}
