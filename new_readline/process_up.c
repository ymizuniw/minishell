/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_up.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:19 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 23:39:33 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_up(t_new_readline *nr)
{
	if (!nr->hist || nr->hist->size == 0)
		return (0);
	if (nr->hist->cur == -1)
		nr->hist->cur = (nr->hist->idx + HIST_MAX - 1) % HIST_MAX;
	else
		nr->hist->cur = (nr->hist->cur + HIST_MAX - 1) % HIST_MAX;
	if (nr->hist->hist_box[nr->hist->cur] == NULL)
		return (0);
	copy_history_to_buffer(nr);
	redraw_line(nr);
	return (1);
}
