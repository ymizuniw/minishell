/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_word.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 12:06:03 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	**resize_result(char **res, size_t old_count, size_t new_count)
{
	char	**new_res;
	size_t	old_size;

	old_size = sizeof(char *) * (old_count + 1);
	new_res = ft_realloc(res, old_size, sizeof(char *) * (old_count + new_count
				+ 1));
	return (new_res);
}

static int	copy_wildcards(char **res, char **wildcards, size_t *i,
		size_t count)
{
	size_t	j;

	j = 0;
	while (j < count)
	{
		res[*i] = ft_strdup(wildcards[j]);
		if (!res[*i])
			return (0);
		(*i)++;
		j++;
	}
	return (1);
}

static int	handle_no_wildcard(t_word *word, char ***res, size_t *i,
		char **wildcard_arr)
{
	free_double_array(wildcard_arr);
	(*res)[*i] = ft_strdup(word->word);
	if (!(*res)[*i])
		return (0);
	(*i)++;
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
	if (wildcard_count == 0)
		return (handle_no_wildcard(word, res, i, wildcard_arr));
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
