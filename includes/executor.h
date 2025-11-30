/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:15 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/30 22:17:55 by ymizuniw         ###   ########.fr       */
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

// heredoc utils
unsigned int	ft_rand(unsigned int *seed);
int				ft_mkstmpfd(void);
int				make_heredoc(t_redir *hd, t_shell *shell);
int				get_document(t_redir *hd, char **document, size_t *document_len,
					t_shell *shell);
char			*heredoc_value_expansion(const char *line, bool in_quote,
					size_t len, t_shell *shell);
t_word  *handle_quoted_word(const char *input, size_t *idx, char quote);
t_word  *handle_unclosed_quote(const char *input, size_t input_len, size_t *idx);
t_word  *handle_doller_word(const char *input, size_t input_len, size_t *idx);
t_word  *handle_unquoted_word(const char *input, size_t input_len, size_t *idx);
t_word  *append_node(t_word *head, t_word *new);

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
char *expand_plain_word(t_word *list, t_shell *shell);
// char			*expand_word(char *word, t_shell *shell);
char			*ft_itoa(int n);

// Word expansion utilities
size_t			count_word_list(t_word *word);
char			*expand_doller(t_word *word, t_shell *shell);
int				expand_wildcard_to_result(t_word *word, char ***res, size_t *i,
					size_t *total_count);
int				get_document(t_redir *hd, char **document, size_t *document_len,
					t_shell *shell);
t_word			*loop_wrapper(char *value, size_t *i);

// process heredoc
int				make_pipe_heredoc(char *document, size_t document_len);
int				process_one_heredoc(t_shell *shell, t_redir *redir);
int				process_all_heredoc(t_shell *shell, t_ast *node);
int				generate_random_template(char *filename);

//gen_word
char	*expand_token_words(t_word *word_list, t_shell *shell);
char	**expand_token_with_wildcard(t_word *word_list, t_shell *shell,
			size_t *result_count);
bool	has_wildcard_to_expand(t_word *word_list);
t_word	*gen_word(char *value, size_t value_len, size_t *addition);

#endif
