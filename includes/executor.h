/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:15 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/21 22:37:12 by ymizuniw         ###   ########.fr       */
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
char			**ft_expand_word(t_token **tokens, size_t token_count,
					t_shell *shell);
char			*expand_word(char *word, t_shell *shell);
char			*ft_itoa(int n);
int				append_to_expanded(char **expanded, size_t *len,
					const char *str, size_t str_len);
int				expand_single_dollar(char **word_ptr, char **expanded,
					size_t *len, t_shell *shell);

// Word expansion utilities
size_t			count_word_list(t_word *word);
char			*expand_doller(t_word *word, t_shell *shell);
int				add_plain_word(t_word *word, char **res, size_t i);
int				add_dollar_expansion(t_word *word, char **res, size_t i,
					t_shell *shell);
int				expand_wildcard_to_result(t_word *word, char ***res, size_t *i,
					size_t *total_count);
int				get_document(t_redir *hd, char **document, size_t *document_len,
					t_shell *shell);
t_word			*loop_wrapper(char *value, size_t *i);

// process heredoc
int				process_one_heredoc(t_shell *shell, t_redir *redir);
int				process_all_heredoc(t_shell *shell, t_ast *node);
int				generate_random_template(char *filename);

#endif
