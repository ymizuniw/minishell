/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:01:15 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/01 01:25:21 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	t_hist	hist;
	char	*line;

	memset(&hist, 0, sizeof(t_hist));
	line = NULL;
	while (1)
	{
		line = new_readline(&hist, false, "shell$");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		printf("READ: \"%s\"\n", line);
		add_history(line, &hist);
		free(line);
	}
	return (0);
}
