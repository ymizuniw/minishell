#ifndef LEXER_H
# define LEXER_H

# include "minishell_types.h"

// lexer utils
int				check_unclosed_quotes(const char *input);
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
// size_t			word_cat(t_word_cat *, size_t word_len);

#endif
