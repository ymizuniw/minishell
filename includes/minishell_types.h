/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:31 by kemotoha          #+#    #+#             */
/*   Updated: 2025/11/30 21:05:13 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

# include "../libft/libft.h"
# include "new_readline.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

// heredoc pipe capacity from system default size.
# define HERE_PIPE_SIZE 4096
// # define HERE_TEMPLATE "/tmp/heredoc_tmp_XXXXX"

// ECHOCTL value is depending on executing evironment.
# ifndef ECHOCTL
#  define ECHOCTL 0001000
# endif

# define SYNTAX_ERR_TEMPLATE "minishell: syntax error near unexpected token `"
# define SYNTAX_ERR_TEMPLATE_LEN 47
# define UNEXPECTED_EOF_MSG \
	"minishell: syntax error: \
unexpected EOF while looking for matching ')'\n"
# define UNEXPECTED_RPAREN_MSG \
	"minishell: syntax error\
 near unexpected token ')'\n"

# define SAFE_CHARSET "ABCDEFGHIJKLMNOPQRSTUVWXYZ\
	abcdefghijklmnopqrstuvwxyz0123456789-_"

# define TOKEN_VALUE_PIPE "|"
# define TOKEN_VALUE_AND_IF "&&"
# define TOKEN_VALUE_OR_IF "||"
# define TOKEN_VALUE_LPAREN "("
# define TOKEN_VALUE_RPAREN ")"
# define TOKEN_VALUE_REDIRECT_IN "<"
# define TOKEN_VALUE_REDIRECT_OUT ">"
# define TOKEN_VALUE_HEREDOC "<<"
# define TOKEN_VALUE_APPEND ">>"

# define ASC_ETX 3
# define ASC_EOT 4
# define ASC_DEL 127
# define ASC_ESC 27

typedef struct s_shell	t_shell;

// naming is bad. this is variable?
typedef struct s_expand_ctx
{
	t_shell				*shell;
	size_t				word_list_count;
}						t_expand_ctx;

typedef struct s_env
{
	char				*key;
	char				*value;
	int					exported;
	struct s_env		*next;
}						t_env;

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
}						t_metachar;

// token_type classifies which type the token belongs to.
typedef enum e_token_type
{
	TK_HEAD,
	TK_WORD,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_HEREDOC,
	TK_APPEND,
	TK_AND_IF,
	TK_OR_IF,
	TK_LPAREN,
	TK_RPAREN,
	TK_DOLLER,
	TK_EOF
}						t_token_type;

typedef struct s_token
{
	size_t				size;
	size_t				count_newline;
	struct s_token		*prev;
	t_token_type		type;
	bool				in_squote;
	bool				in_dquote;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}						t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	REDIR_OTHER
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*filename;
	bool				delim_quoted;
	int					tmp_fd;
	struct s_redir		*next;
}						t_redir;

typedef enum s_word_type
{
	WD_LITERAL,
	WD_DOLLER,
	WD_OTHER
}						t_word_type;

typedef struct s_word
{
	char				*word;
	t_word_type			type;
	bool				to_expand_doller;
	bool				to_expand_wildcard;
	struct s_word		*next;
}						t_word;

typedef struct s_cmd
{
	t_token				**tokens;
	size_t				token_count;
	char				**argv;
	t_redir				*redir;
}						t_cmd;

typedef struct s_ast
{
	struct s_ast		*parent;
	struct s_ast		*subtree;
	struct s_ast		*left;
	t_node_type			type;
	t_cmd				*cmd;
	struct s_ast		*right;
}						t_ast;

typedef struct s_shell
{
	bool				interactive;
	bool				in_pipe_child;
	int					last_exit_status;
	t_env				*env_list;
	char				*pwd;
	t_ast				*root;
	t_token				*token_list;
	char				*line_ptr;
	t_hist				*hist;
	int					stdin_backup;
	int					stdout_backup;
	struct termios		orig_term;
}						t_shell;

typedef struct s_heredoc_ctx
{
	size_t				i;
	t_shell				*shell;
}						t_heredoc_ctx;

typedef struct s_heredoc_params
{
	char				*delim;
	bool				delim_quoted;
	t_shell				*shell;
}						t_heredoc_params;

typedef struct s_match_data
{
	char				**matches;
	size_t				count;
	size_t				cap;
	char				**keys;
	int					only_wild;
}						t_match_data;

typedef struct s_token_result
{
	char				***token_results;
	size_t				*token_result_counts;
	size_t				total_count;
	size_t				i;
}						t_token_result;

typedef struct s_gen_word
{
	t_word				*word;
	size_t				value_len;
	size_t				start;
	unsigned char		quote;
	size_t				close_place;
}						t_gen_word;

typedef struct s_word_cat
{
	char				**word;
	char const			*input;
	size_t				input_len;
	size_t				*idx;
}						t_word_cat;

#endif
