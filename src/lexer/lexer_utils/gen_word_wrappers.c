/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word_wrappers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 15:21:41 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*ext_unit(char *src, size_t start, size_t end);

int	quote_wrapper(t_gen_word *gw, char *value, size_t *i)
{
	char	*content;

	gw->word->to_expand_doller = (gw->quote == '"');
	gw->word->to_expand_wildcard = false;
	content = ext_unit(value, gw->start + 1, gw->close_place);
	if (!content)
		return (-1);
	gw->word->word = content;
	if (ft_strcmp(content, "$") == 0 || (gw->quote == '"' && ft_strchr(content, '$')))
		gw->word->type = WD_DOLLER;
	else
		gw->word->type = WD_LITERAL;
	*i = gw->close_place + 1;
	return (1);
}

static bool	is_special_var_char(char c)
{
	return (c == '?' || c == '*' || c == '@' || c == '#' || c == '$'
		|| c == '!' || ft_isdigit(c));
}

static void	handle_dollar_sign(t_gen_word *gw, char *value, size_t value_len,
				size_t *i)
{
	(*i)++;
	if (*i >= value_len)
	{
		gw->word->type = WD_LITERAL;
		gw->word->to_expand_doller = false;
		return ;
	}
	if (is_special_var_char(value[*i]))
	{
		gw->word->type = WD_DOLLER;
		gw->word->to_expand_doller = true;
		(*i)++;
	}
	else if (ft_isalpha(value[*i]) || value[*i] == '_')
	{
		gw->word->type = WD_DOLLER;
		gw->word->to_expand_doller = true;
		while (*i < value_len && (ft_isalnum(value[*i]) || value[*i] == '_'))
			(*i)++;
	}
	else
	{
		gw->word->type = WD_LITERAL;
		gw->word->to_expand_doller = false;
	}
}

static void	handle_literal_text(t_gen_word *gw, char *value, size_t value_len,
				size_t *i)
{
	gw->word->type = WD_LITERAL;
	while (*i < value_len && !is_quote(value[*i]) && value[*i] != '$')
	{
		if (gw->word->type == WD_DOLLER && value[*i] == '*')
			break ;
		if (gw->word->type != WD_DOLLER && value[*i] == '*')
			gw->word->to_expand_wildcard = true;
		(*i)++;
	}
}

int	doller_literal_wrapper(t_gen_word *gw, char *value, size_t value_len,
				size_t *i)
{
	if (value && value[*i] == '$')
		handle_dollar_sign(gw, value, value_len, i);
	else
		handle_literal_text(gw, value, value_len, i);
	return (1);
}
