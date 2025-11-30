/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:40 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/30 21:01:31 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "builtin.h"
# include "data_management.h"
# include "env_management.h"
# include "executor.h"
# include "lexer.h"
# include "minishell_types.h"
# include "parser.h"
# include "signal_management.h"
# include "wildcard.h"

// shell_loop_utils.c
t_token	*skip_to_command(t_token *token_list);
int		check_syntax_errors(t_token *token_list, t_shell *shell);
int		check_parenthesis_errors(t_token *token_list, t_shell *shell);
void	exec_one_ast(t_ast *ast, t_shell *shell);
void	cleanup_after_line(t_shell *shell);

t_token	*lexer(const char *input);
int		executor(t_ast *ast, t_shell *shell);

// word_cat.c
int		handle_quotation(t_word_cat *ctx, size_t word_len, size_t *consumed_len,
			char quote_open);
int		handle_plain(t_word_cat *ctx, size_t *word_len);
size_t	word_cat(t_word_cat *ctx, size_t word_len);

#endif
