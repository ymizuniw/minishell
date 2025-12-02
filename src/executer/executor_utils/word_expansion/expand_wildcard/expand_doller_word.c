/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_doller_word.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 17:28:05 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:50:45 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*expand_doller_word(const char *var, t_shell *shell)
{
	char	*env_value;

	if (!var || !shell)
		return (ft_strdup(""));
	if (ft_strcmp(var, "?") == 0)
		return (ft_itoa(shell->last_exit_status));
	env_value = get_env_value(shell->env_list, var);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}
