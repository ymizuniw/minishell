/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:23 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/06 20:28:53 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
bool			is_doller_token(const char *value);
void			prepend_tokens(t_token *head, t_token *new);
int				handle_eof(t_token *token_head);
int				handle_internal_separator(t_token *token_head,
					char const *input, size_t *idx);
int				handle_operators_and_words(t_token *token_head,
					char const *input, size_t input_len, size_t *idx);
t_token			*init_lexer_state(size_t *idx, size_t *input_len,
					const char *input);
int				run_lexer_loop(t_token *head, const char *input,
					size_t input_len);
int				finish_lexer(t_token *head);
int				init_token(t_token **token_head);
size_t			get_input_len(char const *input);
int				process_whitespace(t_token *head, const char *input,
					size_t *idx);
int				process_operator_or_word(t_token *head, const char *input,
					size_t len, size_t *idx);
int				finalize_lexer(t_token *head);

#endif
