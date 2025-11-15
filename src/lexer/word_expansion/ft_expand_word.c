/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 11:51:26 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_word	*gen_word(char *value, size_t value_len, size_t *addition);
void	cleanup_token_results(size_t token_count, char ***token_results,
			size_t *token_result_counts);
int		init_token_results(t_token_result *tr, size_t token_count);
void	init_token_results_elem(t_token_result *tr, t_token **tokens,
			size_t token_count);
int		with_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list,
			size_t token_count);
int		without_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list,
			size_t token_count);
bool	has_wildcard_to_expand(t_word *word_list);

static int	copy_token_result(char ***final_res, size_t *k, char **token_result,
		size_t result_count)
{
	size_t	j;

	j = 0;
	while (j < result_count)
	{
		(*final_res)[*k] = ft_strdup(token_result[j]);
		if (!(*final_res)[*k])
			return (-1);
		(*k)++;
		j++;
	}
	return (1);
}

int	build_final_res(char ***final_res, t_token_result *tr, size_t token_count)
{
	size_t	k;

	*final_res = (char **)xcalloc(sizeof(char *) * (tr->total_count + 1));
	if (!*final_res)
	{
		cleanup_token_results(token_count, tr->token_results,
			tr->token_result_counts);
		return (-1);
	}
	k = 0;
	tr->i = 0;
	while (tr->i < token_count)
	{
		if (copy_token_result(final_res, &k, tr->token_results[tr->i],
				tr->token_result_counts[tr->i]) < 0)
		{
			free_double_array(*final_res);
			cleanup_token_results(token_count, tr->token_results,
				tr->token_result_counts);
			return (-1);
		}
		tr->i++;
	}
	(*final_res)[k] = NULL;
	return (1);
}

static int	process_single_token(t_shell *shell, t_token_result *tr,
		t_token **tokens, size_t token_count)
{
	size_t	value_len;
	size_t	addition;
	t_word	*word_list;

	addition = 0;
	init_token_results_elem(tr, tokens, token_count);
	value_len = ft_strlen(tokens[tr->i]->value);
	word_list = gen_word(tokens[tr->i]->value, value_len, &addition);
	if (!word_list)
		return (-1);
	if (has_wildcard_to_expand(word_list))
	{
		if (with_wildcard(shell, tr, word_list, token_count) < 0)
			return (-1);
	}
	else
	{
		if (without_wildcard(shell, tr, word_list, token_count) < 0)
			return (-1);
	}
	tr->total_count += tr->token_result_counts[tr->i];
	return (1);
}

int	expand_loop(t_shell *shell, t_token_result *tr, t_token **tokens,
		size_t token_count)
{
	while (tr->i < token_count)
	{
		if (process_single_token(shell, tr, tokens, token_count) < 0)
		{
			cleanup_token_results(token_count, tr->token_results,
				tr->token_result_counts);
			return (-1);
		}
		tr->i++;
	}
	return (1);
}

char	**ft_expand_word(t_token **tokens, size_t token_count, t_shell *shell)
{
	t_token_result	tr;
	char			**final_res;

	if (init_token_results(&tr, token_count) < 0)
		return (NULL);
	tr.total_count = 0;
	tr.i = 0;
	if (expand_loop(shell, &tr, tokens, token_count) < 0)
		return (NULL);
	if (build_final_res(&final_res, &tr, token_count) < 0)
		return (NULL);
	cleanup_token_results(token_count, tr.token_results,
		tr.token_result_counts);
	return (final_res);
}
