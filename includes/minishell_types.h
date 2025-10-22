#ifndef MINISHELL_TYPES_H
#define MINISHELL_TYPES_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>//printf()
# include <stdlib.h>//malloc()
# include <string.h>//strlcpy()->rem
# include <unistd.h>//write()
# include <stdbool.h>
# include <ctype.h>//isspace()->rem
# include <fcntl.h>//open()

#define SYNTAX_ERR_TEMPLATE "minishell: syntax error near unexpected token `"
#define SYNTAX_ERR_TEMPLATE_LEN 47
#define TOKEN_VALUE_NEWLINE "newline"
#define TOKEN_VALUE_PIPE "|"
#define TOKEN_VALUE_AND_IF "&&"
#define TOKEN_VALUE_OR_IF "||"
#define TOKEN_VALUE_LPAREN "("
#define TOKEN_VALUE_RPAREN ")"
#define TOKEN_VALUE_REDIRECT_IN "<"
#define TOKEN_VALUE_REDIRECT_OUT ">"
#define TOKEN_VALUE_HEREDOC "<<"
#define TOKEN_VALUE_APPEND ">>"

typedef enum e_metachar
{
	MT_OTHER,
	MT_OR,
	MT_AND,
	MT_LPAREN,
	MT_RPAREN,
	MT_LESS,
	MT_GREAT,
	MT_SPACE,
	MT_TAB,
	MT_NEWLINE
}					t_metachar;

// token_type classifies which type the token belongs to.
typedef enum e_token_type
{
	TK_NEWLINE,
	TK_WORD,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_REDIRECT_IN_AND_OUT,
	TK_HEREDOC,
	TK_APPEND,
	TK_AND_IF,
	TK_OR_IF,
	TK_LPAREN,
	TK_RPAREN,
	TK_DOLLER,
	TK_EOF,
	TK_HEAD
}					t_token_type;

typedef struct s_token
{
	size_t size; // for dummy head to keep the len of the list.
	struct s_token	*prev;
	t_token_type	type;
	bool			in_squote;
	bool			in_dquote;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}					t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	bool			delim_quoted;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redir;
}					t_cmd;

typedef struct s_pipeline
{
	bool	in_pipeline;
	int in_fd;
	int out_fd;
	int err_fd;
} t_pipeline;

typedef struct s_ast
{
	struct s_ast	*parent;
	struct s_ast	*subtree;
	struct s_ast	*left;
	t_node_type		type;
	t_pipeline		*pipeline;
	t_cmd			*cmd;
	struct s_ast	*right;
}					t_ast;

typedef struct s_result
{
	t_ast			*root;
	int				exit_code;
}					t_result;

#endif