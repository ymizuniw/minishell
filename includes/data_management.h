#ifndef DATA_MANAGE_H
# define DATA_MANAGE_H

# include "minishell_types.h"

// wrapper for malloc and free
void		*xmalloc(size_t);
void		xfree(void *);

// alloc application
t_token		*alloc_token(void);
t_cmd		*alloc_cmd(void);
t_ast		*alloc_node(void);
t_redir		*alloc_redir(void);
t_argv		*alloc_argv(void);
t_result	*alloc_result(void);

// free application
void		free_ast_tree(t_ast *p);
void		free_subshell_tree(t_ast *p);
void		free_token_list(t_token *p);
void		free_double_array_contents(char **p, char *p_content);
void		free_double_array(char **p);
void		free_redir_list(t_redir *p);
void		free_cmd_structure(t_cmd *p);
void		free_result(t_result *s);

#endif
