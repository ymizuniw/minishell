/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:09:44 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 19:11:08 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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
