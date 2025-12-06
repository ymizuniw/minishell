/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_value_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:33 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 19:11:16 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	append_res(char **res, char c)
{
	char	tmp[2];
	size_t	res_len;

	tmp[0] = c;
	tmp[1] = '\0';
	res_len = 0;
	if (*res)
		res_len = strlen(*res);
	if (!join_value(res, tmp, res_len, 1))
		return (false);
	return (true);
}

static bool	process_dollar(char **res, const char *line, size_t len,
		t_heredoc_ctx *ctx)
{
	if (ctx->i + 1 < len && line[ctx->i + 1] == '?')
		return (expand_exit_status(res, ctx->shell, &ctx->i));
	return (expand_variable(res, line, len, ctx));
}

static bool	process_normal_char(char **res, const char *line,
		t_heredoc_ctx *ctx)
{
	if (!append_res(res, line[ctx->i]))
		return (false);
	ctx->i++;
	return (true);
}

char	*heredoc_value_expansion(const char *line, bool in_quote, size_t len,
		t_shell *shell)
{
	char			*res;
	t_heredoc_ctx	ctx;

	ctx.i = 0;
	ctx.shell = shell;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (ctx.i < len)
	{
		if (line[ctx.i] == '$' && !in_quote)
		{
			if (process_dollar(&res, line, len, &ctx) == false)
			{
				xfree(res);
				return (NULL);
			}
		}
		else if (!process_normal_char(&res, line, &ctx))
		{
			xfree(res);
			return (NULL);
		}
	}
	return (res);
}
