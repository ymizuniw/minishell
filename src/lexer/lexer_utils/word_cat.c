#include "../../../includes/minishell.h"

int	handle_quotation(t_word_cat *ctx, size_t word_len, size_t *consumed_len,
		char quote_open)
{
	size_t	add_len;
	char	*quote_close;

	quote_close = NULL;
	if (ctx->input[*ctx->idx + 1])
		quote_close = ft_strchr(&ctx->input[*ctx->idx + 1], quote_open);
	if (!quote_close)
		return (0);
	add_len = quote_close - &ctx->input[*ctx->idx] - 1;
	*ctx->word = ft_realloc(*ctx->word, sizeof(char) * (word_len + 1),
			sizeof(char) * (word_len + add_len + 1));
	if (!*ctx->word)
		return (-1);
	ft_memcpy(*ctx->word + word_len, &ctx->input[*ctx->idx + 1], add_len);
	(*ctx->word)[add_len] = '\0';
	*ctx->idx += add_len + 2;
	*consumed_len = add_len;
	return (1);
}

int	handle_plain(t_word_cat *ctx, size_t *word_len)
{
	size_t		add_len;
	const char	*tmp_ptr;

	add_len = 0;
	tmp_ptr = &ctx->input[*ctx->idx];
	while (*ctx->idx < ctx->input_len
		&& !ft_isspace((unsigned char)ctx->input[*ctx->idx])
		&& is_meta_char(ctx->input[*ctx->idx]) == MT_OTHER)
		(*ctx->idx)++;
	add_len = &ctx->input[*ctx->idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*ctx->word = ft_realloc(*ctx->word, sizeof(char) * (*word_len + 1),
			sizeof(char) * (*word_len + (add_len) + 1));
	if (!*ctx->word)
		return (-1);
	ft_memcpy(*ctx->word + *word_len, tmp_ptr, add_len);
	(*ctx->word)[*word_len + add_len] = '\0';
	*word_len += add_len;
	return (1);
}

size_t	word_cat(t_word_cat *ctx, size_t word_len)
{
	char	quote;
	size_t	start;

	start = *ctx->idx;
	quote = 0;
	while (*ctx->idx < ctx->input_len)
	{
		if (!quote && is_quote(ctx->input[*ctx->idx]))
			quote = ctx->input[*ctx->idx];
		else if (quote && ctx->input[*ctx->idx] == quote)
			quote = 0;
		else if (!quote && (ft_isspace((unsigned char)ctx->input[*ctx->idx])
				|| is_meta_char(ctx->input[*ctx->idx]) != MT_OTHER))
			break ;
		(*ctx->idx)++;
	}
	if (quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (0);
	}
	*ctx->word = ft_realloc(*ctx->word, sizeof(char) * (word_len + 1),
			sizeof(char) * (word_len + (*ctx->idx - start) + 1));
	if (!*ctx->word)
		return (0);
	ft_memcpy(*ctx->word + word_len, &ctx->input[start], *ctx->idx - start);
	(*ctx->word)[word_len + (*ctx->idx - start)] = '\0';
	return (1);
}
