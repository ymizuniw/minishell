/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:29:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 19:43:20 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

char	**expand_wildcard_branch(t_word *word_list, t_shell *shell)
{
	size_t	count;
	char	**res;

	count = 0;
	res = NULL;
	res = expand_token_with_wildcard(word_list, shell, &count);
	free_word_list(word_list);
	return (res);
}

char	**expand_token(t_token *token, t_shell *shell)
{
	size_t	value_len;
	size_t	additional;
	t_word	*word_list;
	char	**res;

	if (!token || !token->value)
		return (NULL);
	value_len = ft_strlen(token->value);
	additional = 0;
	word_list = gen_word(token->value, value_len, &additional);
	if (!word_list)
		return (NULL);
	if (has_wildcard_to_expand(word_list))
		return (expand_wildcard_branch(word_list, shell));
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
	size_t	i;
	size_t	j;

	argv = NULL;
	argc = 0;
	i = 0;
	j = 0;
	while (i < count)
	{
		expanded = expand_token(tokens[i], shell);
		if (!expanded)
			return (free_double_array(argv), NULL);
		while (expanded[j])
		{
			argv = append_argv(argv, &argc, expanded[j]);
			j++;
		}
		free_double_array(expanded);
		i++;
	}
	return (argv);
}
