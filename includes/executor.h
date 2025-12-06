/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kemotoha <kemotoha@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:15 by kemotoha          #+#    #+#             */
/*   Updated: 2025/12/06 17:40:47 by kemotoha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell_types.h"

// executor entry
int				ast_traversal(t_ast *node, t_shell *shell);

// redirection and command execution helpers
int				do_redirection(t_ast *node, t_shell *shell);
int				exec_command(t_ast *node, t_shell *shell);
int				exec_pipe(t_ast *node, t_shell *shell);
int				exec_subshell(t_ast *node, t_shell *shell);

// ast_traversal_utils
char			**append_argv(char **argv, size_t *argc, char *str);
int				handle_redirection_error(t_ast *node, int stdin_k, int stdout_k,
					t_shell *shell);
int				process_command(t_ast *node, t_shell *shell);
void			restore_stdio(int stdin_keep, int stdout_keep);
// char			**expand_tokens(t_token **tokens, size_t count, t_shell *shell);
char			**expand_token(t_token *token, t_shell *shell);

// heredoc utils
unsigned int	ft_rand(unsigned int *seed);
int				ft_mkstmpfd(void);
int				make_heredoc(t_redir *hd, t_shell *shell);
int				get_document(t_redir *hd, char **document, size_t *document_len,
					t_shell *shell);
char			*heredoc_value_expansion(const char *line, bool in_quote,
					size_t len, t_shell *shell);
t_word			*handle_quoted_word(const char *input, size_t *idx, char quote);
t_word			*handle_unclosed_quote(const char *input, size_t input_len,
					size_t *idx);
t_word			*handle_doller_word(const char *input, size_t input_len,
					size_t *idx);
t_word			*handle_unquoted_word(const char *input, size_t input_len,
					size_t *idx);
t_word			*append_node(t_word *head, t_word *new);
char			*ext_unit(char *s, size_t start, size_t end);
int				join_value(char **d, const char *s, size_t dlen, size_t slen);

// search and exec
void			search_and_exec(t_shell *shell, char **cmd_args);
void			search_in_path_and_exec(t_shell *shell, char **cmd_args);
char			*find_command_path(t_shell *shell, char *cmd);
char			**generate_envp(t_env *env_list);
void			exec_with_slash(t_shell *shell, char **cmd_args, char **env);

void			free_split(char **split);
char			*join_path(char *dir, char *cmd);

void			free_envp(char **envp);
char			*get_env_value(t_env *env_list, const char *key);
char			**generate_envp(t_env *env_list);
char			*create_env_string(const char *key, const char *value);
char			**generate_envp(t_env *env_list);

// Variable expansion (at execution time)
char			*ft_itoa(int n);

char			*expand_doller_word(const char *var, t_shell *shell);
char			*expand_plain_word(t_word *list, t_shell *shell, bool *quoted);
char			*get_word_string(t_word *word);
// Word expansion utilities
size_t			count_word_list(t_word *word);
char			*expand_doller(t_word *word, t_shell *shell);
int				get_document(t_redir *hd, char **document, size_t *document_len,
					t_shell *shell);
t_word			*loop_wrapper(char *value, size_t *i);

// process heredoc
int				make_pipe_heredoc(char *document, size_t document_len);
int				process_one_heredoc(t_shell *shell, t_redir *redir);
int				process_all_heredoc(t_shell *shell, t_ast *node);
int				generate_random_template(char *filename);

// heredoc utils
int				join_value(char **d, const char *s, size_t dlen, size_t slen);
char			*ext_unit(char *s, size_t start, size_t end);

// gen_word
char			*expand_token_words(t_word *word_list, t_shell *shell);
t_word			*gen_word(char *value, size_t value_len, size_t *addition);
t_word			*create_word_node(char *content, t_word_type type,
					bool expand_doller);

// pipe
pid_t			fork_right_process(int pip[2], t_ast *node, t_shell *shell);
pid_t			fork_left_process(int pip[2], t_ast *node, t_shell *shell,
					pid_t right_pid);

#endif
