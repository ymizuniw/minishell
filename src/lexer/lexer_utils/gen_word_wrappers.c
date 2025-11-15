/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gen_word_wrappers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 11:51:26 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*ext_unit(char *src, size_t start, size_t end);

int	quote_wrapper(t_gen_word *gw, char *value, size_t *i)
{
	char	*content;

	if (gw->quote == '\'')
		gw->word->to_expand_doller = false;
	else
		gw->word->to_expand_doller = true;
	gw->word->to_expand_wildcard = false;
	content = ext_unit(value, gw->start + 1, gw->close_place);
	if (!content)
		return (-1);
	gw->word->word = content;
	if (gw->quote == '"' && ft_strchr(content, '$'))
		gw->word->type = WD_DOLLER;
	else if (gw->quote == '"' && content[0] == '$' && content[1] != '\0')
		gw->word->type = WD_DOLLER;
	else
		gw->word->type = WD_LITERAL;
	*i = gw->close_place + 1;
	return (1);
}

static int	correct_env_var_format(char c)
{
	if (c == '?' || c == '*' || c == '@' || c == '#' || c == '$'
		|| c == '!' || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static void	handle_dollar_sign(t_gen_word *gw, char *value, size_t value_len,
		size_t *i)
{
	(*i)++;
	if (*i < value_len && correct_env_var_format(value[*i]))
	{
		gw->word->type = WD_DOLLER;
		gw->word->to_expand_doller = true;
		(*i)++;
	}
	else if (*i < value_len && (ft_isalpha(value[*i]) || value[*i] == '_'))
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

t_word	*loop_wrapper(char *value, size_t value_len, size_t *i)
{
	t_word		*word;
	t_gen_word	gw;
	int			init_gen_word_data(t_word **word, t_gen_word *gw,
					char *value, size_t *i);
	size_t		quote_close_place(char quote, char *value, size_t i);

	if (init_gen_word_data(&word, &gw, value, i) < 0)
		return (NULL);
	gw.start = *i;
	if (gw.quote != '\0')
		gw.close_place = quote_close_place(gw.quote, value, ++(*i));
	if (gw.quote != '\0' && gw.close_place > 0)
	{
		if (quote_wrapper(&gw, value, i) < 0)
			return (xfree(word), NULL);
	}
	else
	{
		doller_literal_wrapper(&gw, value, value_len, i);
		word->word = ext_unit(value, gw.start, *i);
		if (!word->word)
			return (xfree(word), NULL);
	}
	return (word);
}
