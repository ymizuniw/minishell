#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif
# include <ctype.h> //isspace()->rem
# include <dirent.h>
# include <fcntl.h> //open()
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>   //printf()
# include <stdlib.h>  //malloc()
# include <string.h>  //strncpy()->rem
# include <strings.h> //bzero()
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h> //write()

// heredoc pipe capacity from system default size.
# define HERE_PIPE_SIZE 4096
// HEREDOC TMPFILE'S TEMPLATE
# define HERE_TEMPLATE "/tmp/heredoc_tmp_XXXXX"

# define SYNTAX_ERR_TEMPLATE "minishell: syntax error near unexpected token `"
# define SYNTAX_ERR_TEMPLATE_LEN 47
# define TOKEN_VALUE_NEWLINE "newline"
# define TOKEN_VALUE_PIPE "|"
# define TOKEN_VALUE_AND_IF "&&"
# define TOKEN_VALUE_OR_IF "||"
# define TOKEN_VALUE_LPAREN "("
# define TOKEN_VALUE_RPAREN ")"
# define TOKEN_VALUE_REDIRECT_IN "<"
# define TOKEN_VALUE_REDIRECT_OUT ">"
# define TOKEN_VALUE_HEREDOC "<<"
# define TOKEN_VALUE_APPEND ">>"

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	bool			interactive;
	int				last_exit_status;
	t_env			*env_list;
	// t_command_hash *cmd_hash;
	char			*pwd;
}					t_shell;

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
	TK_HEAD,
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
	TK_EOF
}					t_token_type;

typedef struct s_token
{
	size_t size; // for dummy head to keep the len of the list.
	size_t			count_newline;
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
	REDIR_HEREDOC,
	REDIR_OTHER
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	bool			delim_quoted;
	struct s_redir	*next;
}					t_redir;

typedef struct s_argv
{
	char			*word;
	bool			to_expand;
	struct s_argv	*next;
}					t_argv;

typedef struct s_cmd
{
	t_argv			*argv_list;
	char			**argv;
	t_redir			*redir;
}					t_cmd;

typedef struct s_ast
{
	struct s_ast	*parent;
	struct s_ast	*subtree;
	struct s_ast	*left;
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*right;
}					t_ast;

typedef struct s_result
{
	t_ast			*root;
	int				exit_code;
}					t_result;

#endif
