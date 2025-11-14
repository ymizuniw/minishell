/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:30:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/14 15:17:31 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*get_target_var(t_word *word)
{
	if (word->type == WD_DOLLER)
		return (&word->word[1]);
	return (word->word);
}

static char	*expand_exit_status(t_shell *shell)
{
	char *stat_str;
	
	stat_str = ft_itoa(shell->last_exit_status);
	if (stat_str==NULL)
		return(NULL);
	return (stat_str);
}

char	*expand_doller(t_word *word, t_shell *shell)
{
	char	*env_value;
	char	*target;

	target = get_target_var(word);
	if (ft_strcmp(target, "?") == 0)
		return (expand_exit_status(shell));
	env_value = get_env_value(shell->env_list, target);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}
