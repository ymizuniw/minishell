/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/25 22:57:51 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

// Forward declarations - implementations in separate files
int	handle_newline(size_t *idx);
int	handle_meta_char(t_token *token_head, const char *input, size_t *idx);
int	handle_doller(t_token *token_head, size_t *idx);
int	handle_word(t_token *token_head, char const *input, size_t input_len,
		size_t *idx);
int	handle_word_and_doller(t_token *token_head, char const *input,
		size_t input_len, size_t *idx);
int	handle_eof(t_token *token_head);
int	handle_internal_separator(t_token *token_head, char const *input,
		size_t *idx);
int	handle_operators_and_words(t_token *token_head, char const *input,
		size_t input_len, size_t *idx);
