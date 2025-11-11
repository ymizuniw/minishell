#ifndef DATA_MANAGE_H
# define DATA_MANAGE_H

# include "minishell_types.h"

// wrapper for malloc and free
void		*xmalloc(size_t);
void		*xcalloc(size_t);
void		xfree(void *);

// alloc application
t_token		*alloc_token(void);
t_cmd		*alloc_cmd(void);
t_ast		*alloc_node(void);
t_redir		*alloc_redir(void);
t_result	*alloc_result(void);

// free application
void		free_ast_tree(t_ast *p);
void		free_subshell_tree(t_ast *p);
void		free_token_list(t_token *p);
void		free_double_array_contents(char **p, char *p_content);
void		free_double_array(char **p);
void		free_redir_list(t_redir *p);
void		free_word_list(t_word *p);
void		free_cmd_structure(t_cmd *p);
void		free_result(t_result *s);
void		free_shell(t_shell *shell);

// list utilities
t_token		*token_last(t_token *head);
t_redir		*redir_last(t_redir *head);
t_word		*word_last(t_word *head);
void		redir_add_back(t_redir **head, t_redir *new);
void		word_add_back(t_word **head, t_word *new);
void		token_add_back(t_token **head, t_token *new);

#endif
