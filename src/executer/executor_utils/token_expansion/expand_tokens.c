/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:29:36 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/04 04:29:08 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"


static int ft_isifs(char s)
{
	return (s == ' ' || s == '\t' || s == '\n');
}

static size_t count_word(char *str)
{
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break;
		while (str[i] && !ft_isifs(str[i]))
			i++;
		j++;
	}
	return (j);
}

static int extract_word(char **result, size_t *idx, char *str, size_t *i)
{
	size_t start;

	start = *i;
	while (str[*i] && !ft_isifs(str[*i]))
		(*i)++;
	result[*idx] = ft_substr(str, start, *i - start);
	if (!result[*idx])
	{
		while (*idx > 0)
		{
			(*idx)--;
			free(result[*idx]);
		}
		free(result);
		return (0);
	}
	(*idx)++;
	return (1);
}

char **split_by_ifs(char *str)
{
	size_t i;
	size_t word;
	char **result;

	i = 0;
	word = count_word(str);
	result = xmalloc(sizeof(char *) * (word + 1));
	if (!result)
		return (NULL);
	size_t idx = 0;
	while (str[i])
	{
		while (str[i] && ft_isifs(str[i]))
			i++;
		if (!str[i])
			break;
		if (!extract_word(result, &idx, str, &i))
			return (NULL);
	}
	result[idx] = NULL;
	return (result);
}

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
	bool quoted = false;

	if (!token || !token->value)
		return (NULL);
	value_len = ft_strlen(token->value);
	additional = 0;
	word_list = gen_word(token->value, value_len, &additional);
	if (!word_list)
		return (NULL);
	if (has_wildcard_to_expand(word_list))
		return (expand_wildcard_branch(word_list, shell));
	char *expanded = expand_plain_word(word_list, shell, &quoted);
	free_word_list(word_list);
	if (!expanded)
		return (NULL);
	if (quoted)
	{
		res = xcalloc(sizeof(char *) * 2);
        res[0] = expanded;
        res[1] = NULL;
        return (res);
	}
	res = split_by_ifs(expanded);
	free(expanded);
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
		j = 0;
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
