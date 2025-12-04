/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:39:01 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/05 00:29:58 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	**append_argv(char **argv, size_t *argc, char *str)
{
	char	**new_argv;
	size_t	i;

	if (str == NULL)
		return (NULL);
	new_argv = xmalloc(sizeof(char *) * (*argc + 2));
	if (new_argv == NULL)
		return (NULL);
	i = 0;
	while (i < *argc)
	{
		new_argv[i] = argv[i];
		i++;
	}
	new_argv[*argc] = ft_strdup(str);
	if (new_argv[*argc] == NULL)
	{
		xfree(argv);
		return (NULL);
	}
	new_argv[*argc + 1] = NULL;
	xfree(argv);
	(*argc)++;
	return (new_argv);
}
