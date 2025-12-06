/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_value_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:37:33 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 17:54:03 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	expand_exit_status(char **res, t_shell *shell, size_t *i)
{
	char	*s;
	size_t	res_len;
	size_t	s_len;

	s = ft_itoa(shell->last_exit_status);
	if (!s)
	{
		xfree(*res);
		return (false);
	}
	res_len = 0;
	s_len = 0;
	if (*res)
		res_len = ft_strlen(*res);
	if (s)
		s_len = ft_strlen(s);
	if (!join_value(res, s, res_len, s_len))
	{
		xfree(s);
		xfree(*res);
		return (false);
	}
	xfree(s);
	*i += 2;
	return (true);
}

static bool	cat_env_value(char **res, const char *val)
{
	size_t	res_len;
	size_t	val_len;

	res_len = 0;
	val_len = 0;
	if (*res)
		res_len = ft_strlen(*res);
	if (val)
		val_len = ft_strlen(val);
	if (!join_value(res, val, res_len, val_len))
		return (false);
	return (true);
}

void	set_val(t_env *env_entry, const char **val, t_heredoc_ctx *ctx,
		char *varname)
{
	env_entry = find_env(ctx->shell->env_list, varname);
	if (env_entry && env_entry->value)
		*val = env_entry->value;
	else
		*val = "";
}

bool	expand_variable(char **res, const char *line, size_t len,
		t_heredoc_ctx *ctx)
{
	size_t		start;
	char		*varname;
	const char	*val;
	t_env		*env_entry;

	env_entry = NULL;
	start = ++ctx->i;
	while (ctx->i < len && (ft_isalnum((unsigned char)line[ctx->i])
			|| line[ctx->i] == '_'))
		ctx->i++;
	varname = ext_unit((char *)line, start, ctx->i);
	if (!varname)
	{
		xfree(*res);
		return (false);
	}
	set_val(env_entry, &val, ctx, varname);
	if (!cat_env_value(res, val))
	{
		xfree(varname);
		xfree(*res);
		return (false);
	}
	xfree(varname);
	return (false);
}

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
