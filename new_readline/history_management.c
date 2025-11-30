/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:28 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/30 22:23:48 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

void	add_history(char *line, t_hist *hist)
{
	if (!line || !*line || hist==NULL)
		return ;
	if (hist && hist->hist_box[hist->idx] != NULL)
		free(hist->hist_box[hist->idx]);
	hist->hist_box[hist->idx] = strdup(line);
	hist->idx = (hist->idx + 1) % HIST_MAX;
	if (hist->size < HIST_MAX)
		hist->size++;
	hist->cur = -1;
}

void	free_hist_box(char *hist_box[HIST_MAX])
{
	size_t	i;

	i = 0;
	while (i < HIST_MAX)
	{
		free(hist_box[i]);
		hist_box[i] = NULL;
		i++;
	}
}
