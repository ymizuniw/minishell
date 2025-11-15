/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:40 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/15 19:20:35 by ymizuniw         ###   ########.fr       */
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
# include <assert.h>
// static definition needed.

// main.c
char		*ft_readline(t_shell *shell, const char *prompt, t_hist *hist);
void		add_history(char *line, t_hist *hist);
void		free_hist_box(char *hist_box[HIST_MAX]);

// readline utils
void		enable_raw_mode(struct termios *orig);
void		disable_raw_mode(const struct termios *orig);
ssize_t		read_key(char *c);
void		redraw_line(const char *prompt, const char *buf);
void		handle_backspace(char *buf, size_t *len);
void		handle_printable(char *buf, size_t *len, char c);
void		handle_arrow_up(t_hist *hist, char *buf, size_t *len,
				const char *prompt);
void		handle_arrow_down(t_hist *hist, char *buf, size_t *len,
				const char *prompt);
void		handle_escape_sequence(char *seq, t_hist *hist, char *buf,
				t_readline_ctx *ctx);

// shell_loop_utils.c
t_token		*skip_to_command(t_token *token_list);
int			check_syntax_errors(t_token *token_list, t_shell *shell);
int			check_parenthesis_errors(t_token *token_list, t_shell *shell);
void		exec_one_ast(t_ast *ast, t_shell *shell);
void		cleanup_after_line(t_shell *shell);

t_token		*lexer(const char *input);
t_result	executor(t_ast *ast, t_shell *shell);

// word_cat.c
int			handle_quotation(t_word_cat *ctx, size_t word_len,
				size_t *consumed_len, char quote_open);
int			handle_plain(t_word_cat *ctx, size_t *word_len);
size_t		word_cat(t_word_cat *ctx, size_t word_len);

#endif
