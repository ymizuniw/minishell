/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_word_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:28:39 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 18:45:46 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*get_word_string(t_word *word)
{
	if (!word)
		return (ft_strdup(""));
	if (word->type != WD_OTHER)
		return (ft_strdup(word->word));
	return (ft_strdup(""));
}
