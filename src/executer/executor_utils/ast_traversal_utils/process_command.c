/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:32:51 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 16:29:26 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	append_argv_loop(char ***argv, size_t *argc, char ***expanded)
{
	size_t	j;
	char	**new_argv;

	j = 0;
	if (expanded == NULL || *expanded == NULL)
		return (1);
	while ((*expanded)[j])
	{
		new_argv = append_argv(*argv, argc, (*expanded)[j]);
		if (new_argv == NULL)
		{
			free_double_array(*expanded);
			free_double_array(*argv);
			*argv = NULL;
			return (-1);
		}
		*argv = new_argv;
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
	int		ret;

	argv = NULL;
	ret = 0;
	ret = expand_token_loop(node, shell, &argv);
	if (!ret || !argv || !argv[0])
	{
		printf("expand_token_loop failed\n");
		free_double_array(argv);
		return (0);
	}
	search_and_exec(shell, argv);
	free_double_array(argv);
	return (1);
}
