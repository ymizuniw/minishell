#include "../../../includes/minishell.h"

static void	copy_history_to_buffer(t_hist *hist, char *buf, size_t *len)
{
	if (strlen(hist->hist_box[hist->cur]) > 1023)
		return ;
	strncpy(buf, hist->hist_box[hist->cur], 1023);
	buf[1023] = '\0';
	*len = strlen(buf);
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
