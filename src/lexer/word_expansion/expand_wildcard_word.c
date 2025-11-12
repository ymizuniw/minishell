/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/12 10:27:17 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	**resize_result(char **res, size_t old_count, size_t new_count)
{
	char	**new_res;

	new_res = realloc(res, sizeof(char *) * (old_count + new_count + 1));
	return (new_res);
}

static int	copy_wildcards(char **res, char **wildcards, size_t *i,
		size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		res[*i] = strdup(wildcards[j]);
		if (!res[*i])
			return (0);
		(*i)++;
		j++;
	}
	return (1);
}

int	expand_wildcard_to_result(t_word *word, char ***res, size_t *i,
		size_t *total_count)
{
	char	**wildcard_arr;
	size_t	wildcard_count;
	char	**new_res;

	wildcard_count = 0;
	wildcard_arr = expand_wildcard(word->word, "./", &wildcard_count);
	if (!wildcard_arr)
		return (0);
	new_res = resize_result(*res, *total_count, wildcard_count);
	if (!new_res)
		return (free_double_array(wildcard_arr), 0);
	*res = new_res;
	if (!copy_wildcards(*res, wildcard_arr, i, wildcard_count))
		return (free_double_array(wildcard_arr), 0);
	*total_count += wildcard_count;
	free_double_array(wildcard_arr);
	return (1);
}
