#ifndef LXER_H
# define LXER_H

# include "minishell_types.h"

// lexer utils
char			*ext_unit(char *src, size_t start, size_t end);
int				join_value(char **res, const char *value, size_t size1,
					size_t size2);
unsigned char	is_quote(int c);
t_token_type	get_token_type(char *input, size_t *idx);
void			set_token_type(t_token *token, char *input, size_t *tmp_idx);
void			set_quote_flag(t_token *token, char *input,
					char const quote_open);
t_metachar		is_meta_char(int c);
bool			is_doller_token(const char *);
void			prepend_tokens(t_token *head, t_token *new);
size_t			word_cat(char **, size_t, char const *, size_t, size_t *,
					bool *had_sq, bool *had_dq, bool *had_unq);
t_word			*gen_word(char *value, size_t value_len, size_t *addition);

#endif
