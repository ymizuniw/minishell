#ifndef PARSER_H
# define PARSER_H

# include "minishell_types.h"

void	set_argv(char **argv, t_token **start_token, size_t i);
t_ast	*gen_tree(t_ast *ast, t_token **token_ptr, int subshell, int pipeline);

int		check_parenthesis(t_token *token);
int		syntax_check(t_token *token);
t_redir	*parse_redirection(t_redir *redir, t_token **tail_token);
void	cat_word(char **word, char *entry, size_t entry_len);
char	*expand_value(t_token *token);
void	syntax_error(t_token_type type);

#endif