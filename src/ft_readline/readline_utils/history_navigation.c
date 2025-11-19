/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_navigation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:31 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/19 20:05:31 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	copy_history_to_buffer(t_hist *hist, char *buf, size_t *len)
{
	size_t	hist_len;

	hist_len = 0;
	if (hist->hist_box[hist->cur])
		hist_len = ft_strlen(hist->hist_box[hist->cur]);
	if (hist_len > 1023)
		return ;
	ft_strlcpy(buf, hist->hist_box[hist->cur], 1024);
	*len = 0;
	if (buf)
		*len = ft_strlen(buf);
}

void	handle_arrow_up(t_hist *hist, char *buf, size_t *len,
		const char *prompt)
{
	if (hist->size == 0)
		return ;
	if (hist->cur == -1)
		hist->cur = (hist->idx + HIST_MAX - 1) % HIST_MAX;
	else
		hist->cur = (hist->cur + HIST_MAX - 1) % HIST_MAX;
	if (hist->hist_box[hist->cur] == NULL)
		return ;
	copy_history_to_buffer(hist, buf, len);
	redraw_line(prompt, buf);
}

void	handle_arrow_down(t_hist *hist, char *buf, size_t *len,
		const char *prompt)
{
	if (hist->size == 0 || hist->cur == -1)
		return ;
	hist->cur = (hist->cur + 1) % HIST_MAX;
	if ((size_t)hist->cur == hist->idx)
	{
		buf[0] = '\0';
		*len = 0;
		hist->cur = -1;
	}
	else if (hist->hist_box[hist->cur])
		copy_history_to_buffer(hist, buf, len);
	redraw_line(prompt, buf);
}

void	handle_escape_sequence(char *seq, t_hist *hist, char *buf,
		t_readline_ctx *ctx)
{
	if (seq[0] == '[')
	{
		if (seq[1] == 'A')
			handle_arrow_up(hist, buf, ctx->len, ctx->prompt);
		else if (seq[1] == 'B')
			handle_arrow_down(hist, buf, ctx->len, ctx->prompt);
	}
}
