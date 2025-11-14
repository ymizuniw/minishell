/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_plain_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/14 15:06:17 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	add_plain_word(t_word *word, char **res, size_t i)
{
	res[i] = ft_strdup(word->word);
	if (!res[i])
		return (0);
	return (1);
}

int	add_dollar_expansion(t_word *word, char **res, size_t i, t_shell *shell)
{
	char	*env_expand;

	env_expand = expand_doller(word, shell);
	if (!env_expand)
		env_expand = ft_strdup("");
	res[i] = env_expand;
	if (!res[i])
		return (0);
	return (1);
}
