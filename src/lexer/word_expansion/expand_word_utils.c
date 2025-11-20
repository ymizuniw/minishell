/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/20 16:34:34 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*expand_token_words(t_word *word_list, t_shell *shell);
char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
			size_t *result_count);
bool	has_wildcard_to_expand(t_word *word_list);
t_word	*gen_word(char *value, size_t value_len, size_t *addition);

void	cleanup_token_results(size_t token_count, char ***token_results,
		size_t *token_result_counts)
{
	size_t	i;

	i = 0;
	while (i < token_count)
	{
		if (token_results[i])
			free_double_array(token_results[i]);
		i++;
	}
	xfree(token_results);
	xfree(token_result_counts);
}

int	init_token_results(t_token_result *tr, size_t token_count)
{
	tr->token_results = (char ***)xcalloc(sizeof(char **) * token_count);
	tr->token_result_counts = (size_t *)xcalloc(sizeof(size_t) * token_count);
	if (!tr->token_results || !tr->token_result_counts)
		return (xfree(tr->token_results), xfree(tr->token_result_counts), -1);
	return (1);
}

void	init_token_results_elem(t_token_result *tr, t_token **tokens,
		size_t token_count)
{
	while (tr->i < token_count)
	{
		if (!tokens[tr->i] || !tokens[tr->i]->value)
		{
			tr->token_results[tr->i] = (char **)xcalloc(sizeof(char *) * 2);
			if (!tr->token_results[tr->i])
			{
				cleanup_token_results(token_count, tr->token_results,
					tr->token_result_counts);
				return ;
			}
			tr->token_results[tr->i][0] = ft_strdup("");
			tr->token_results[tr->i][1] = NULL;
			tr->token_result_counts[tr->i] = 1;
			tr->total_count += 1;
			(tr->i)++;
			continue ;
		}
		else
			break ;
	}
}

int	with_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list,
		size_t token_count)
{
	tr->token_results[tr->i] = expand_token_with_wildcard(word_list, shell,
			&tr->token_result_counts[tr->i]);
	free_word_list(word_list);
	if (!tr->token_results[tr->i])
	{
		cleanup_token_results(token_count, tr->token_results,
			tr->token_result_counts);
		return (-1);
	}
	return (1);
}

int	without_wildcard(t_shell *shell, t_token_result *tr, t_word *word_list,
		size_t token_count)
{
	tr->token_results[tr->i] = (char **)xcalloc(sizeof(char *) * 2);
	if (!tr->token_results[tr->i])
	{
		free_word_list(word_list);
		cleanup_token_results(token_count, tr->token_results,
			tr->token_result_counts);
		return (-1);
	}
	tr->token_results[tr->i][0] = expand_token_words(word_list, shell);
	tr->token_results[tr->i][1] = NULL;
	free_word_list(word_list);
	if (!tr->token_results[tr->i][0])
	{
		cleanup_token_results(token_count, tr->token_results,
			tr->token_result_counts);
		return (-1);
	}
	tr->token_result_counts[tr->i] = 1;
	return (1);
}
