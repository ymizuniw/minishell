/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:37 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/29 01:06:40 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"


void	handle_backspace(t_readline_ctx *ctx)
{
	if (*ctx->len == 0 || ctx->cursor == 0)
		return ;
	char delete = ctx->buf[*ctx->len - 1];
	*(ctx->len) -= 1;
	ctx->buf[*ctx->len] = '\0';
	if (delete == '\t')
	{
		int pos = ctx->cursor - 1;
		int del_size = 8 - (pos % 8);
		if (del_size==0)
			del_size = 8;
		for (int i =0; i<del_size;i++)
			write(1, "\b \b", 3);
		ctx->cursor -= del_size;
	}
	else
	{
		write(STDOUT_FILENO, "\b \b", 3);
		ctx->cursor--;
	}
}

void	handle_printable(t_readline_ctx *ctx, char c)
{
	if (*ctx->len >= 1023)
		return ;
	ctx->buf[*ctx->len] = c;
	(*ctx->len)++;
	ctx->buf[*ctx->len] = '\0';
	write(STDOUT_FILENO, &c, 1);
	if (c == '\t')
		ctx->cursor += 8 - (ctx->cursor%8);
	else
		ctx->cursor++;
}


// https://www.gnu.org/software/termutils/manual/termcap-1.3/html_mono/termcap.html#SEC16

// #include <termcap.h>

// char term_buffer[2048];
// char *cm, *le, *nd, *ce;

// void init_termcap(void)
// {
//     char *termtype = getenv("TERM");
//     if (!termtype) return;

//     tgetent(term_buffer, termtype);

//     cm = tgetstr("cm", NULL);
//     le = tgetstr("le", NULL);
//     nd = tgetstr("nd", NULL);
//     ce = tgetstr("ce", NULL);
// }
