#ifndef PARSER_H
# define PARSER_H

# include "minishell_types.h"

// Public parser API
t_ast	*parse(t_token **token);
t_ast	*parser(t_token **token);

// Token classification helpers
int		token_is_operator(t_token_type type);
int		token_is_subshell_close(t_token_type type);
int		token_is_command(t_token_type type);
int		token_is_redirection(t_token_type type);
int		token_is_newline_or_eof(t_token_type type);

// Syntax validation
int		check_parenthesis(t_token *token);
int		check_parenthesis_balance(t_token *token_list);
int		syntax_check(t_token *token);
void	syntax_error(t_token_type type);

// AST construction (exposed for cross-file parser units and tests)
void	gen_tree(t_ast **ast, t_token **token);
void	gen_tree_subshell(t_ast **ast, t_token **token);
t_ast	*gen_command_node(t_ast *ast, t_token **token);
t_ast	*gen_subshell_node(t_ast *ast, t_token **token);
t_ast	*swap_with_parent(t_ast **ast, t_token **token);
t_ast	*gen_eof_newline_node(t_ast *ast, t_token **token);
int		parse_redirection(t_redir **redir, t_token_type type, t_token *token);
int		parse_simple_command(t_cmd *cmd, t_token *command_token);
int		parse_command_list(t_cmd *cmd, t_token **token);

// other utils
void	append_redir(t_redir **redir_head, t_redir *new_redir);
void	set_redir_type(t_redir *new_redir, t_token_type token_type);

#endif
