/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:49 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/28 03:55:20 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"
#include "../../includes/minishell.h"

static int	process_key(char c, t_readline_ctx *ctx)
{
	char	seq[2];

	if (c == ASC_ETX)
		return (1);
	if (c == '\n')
		return (0);
	if (c == ASC_EOT && *ctx->len == 0)
		return (-1);
	if (c == ASC_DEL)
		handle_backspace(ctx);
	else if (c == ASC_ESC)
	{
		if (read_key(&seq[0]) <= 0 || read_key(&seq[1]) <= 0)
			return (1);
		handle_escape_sequence(seq, ctx->hist, ctx->buf, ctx);
	}
	else if (ft_isprint((int)c) || ft_isspace((int)c))
		handle_printable(ctx, c);
	return (1);
}

static char	*read_non_interactive(void)
{
	char	*buf;
	size_t	len;

	buf = get_next_line(STDIN_FILENO);
	if (!buf)
		return (NULL);
	len = ft_strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	return (buf);
}
 
static int	read_loop(t_readline_ctx *ctx)
{
	char	c;
	int		result;

	while (read_key(&c) > 0)
	{
		//if len is within 4096, process it.
		//if not, if backspace comes process else do noting and continue.
		if (*ctx->len>1023
			&& c !=ASC_DEL
			&& c!='\n'
			&& c!=ASC_ETX
			&& !(c==ASC_EOT && *ctx->len==0))
			continue ;
		result = process_key(c, ctx);
		if (result == 0)
			return (0);
		else if (result == -1)
			return (-1);
	}
	return (0);
}

static t_readline_ctx	init_readline_ctx(t_hist *hist,
		char const *prompt)
{
	t_readline_ctx	new;

	ft_memset(new.buf, 0, 4096);
	new.cursor = 0;
	new.hist = hist;
	new.prompt = prompt;
	return (new);
}

char	*ft_readline(t_shell *shell, const char *prompt, t_hist *hist)
{
	t_readline_ctx	ctx;
	char *line;

	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return (read_non_interactive());
	ctx = init_readline_ctx(hist, prompt);
	if (shell)
		enable_raw_mode(&shell->orig_term);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	if (read_loop(&ctx) == -1)
		return (disable_raw_mode(&shell->orig_term), NULL);
	write(STDOUT_FILENO, "\n", 1);
	if (shell)
		disable_raw_mode(&shell->orig_term);
	if (hist != NULL && ctx.len > 0)
		add_history(ctx.buf, hist);
	line = ft_strdup(ctx.buf);
	return (line);
}
