/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:32:51 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 17:51:51 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	process_command(t_ast *node, t_shell *shell)
{
	char	**argv;
	size_t	argc;
	char	**expanded;

	argv = NULL;
	argc = 0;
	for (size_t i = 0; i < node->cmd->token_count; i++)
	{
		expanded = expand_token(node->cmd->tokens[i], shell);
		if (expanded == NULL)
			return (free_double_array(argv), 0);
		for (size_t j = 0; expanded[j]; j++)
		{
			argv = append_argv(argv, &argc, expanded[j]);
		}
		free_double_array(expanded);
	}
	if (!argv || !argv[0])
	{
		free_double_array(argv);
		return (0);
	}
	search_and_exec(shell, argv);
	free_double_array(argv);
	return (1);
}
