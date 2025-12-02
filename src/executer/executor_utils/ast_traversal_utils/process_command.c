/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:32:51 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 19:43:36 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	append_argv_loop(char ***argv, size_t *argc, char ***expanded)
{
	size_t	j;

	j = 0;
	if (expanded == NULL || *expanded == NULL)
		return (0);
	while ((*expanded)[j])
	{
		*argv = append_argv(*argv, argc, (*expanded)[j]);
		if (argv == NULL)
		{
			free_double_array(*expanded);
			return (-1);
		}
		j++;
	}
	free_double_array(*expanded);
	return (1);
}

int	expand_token_loop(t_ast *node, t_shell *shell, char ***argv)
{
	char	**expanded;
	size_t	i;
	size_t	argc;

	expanded = NULL;
	i = 0;
	argc = 0;
	while (i < node->cmd->token_count)
	{
		expanded = expand_token(node->cmd->tokens[i], shell);
		if (expanded == NULL)
		{
			free_double_array(*argv);
			return (0);
		}
		if (append_argv_loop(argv, &argc, &expanded) < 0)
		{
			free_double_array(*argv);
			return (0);
		}
		i++;
	}
	return (1);
}

int	process_command(t_ast *node, t_shell *shell)
{
	char	**argv;

	argv = NULL;
	expand_token_loop(node, shell, &argv);
	if (!argv || !argv[0])
	{
		free_double_array(argv);
		return (0);
	}
	search_and_exec(shell, argv);
	free_double_array(argv);
	return (1);
}
