#include "../../includes/minishell.h"

// Forward declarations to avoid implicit prototypes
int		token_is_redirection(t_token_type type);
int		token_is_operator(t_token_type type);
int		token_is_subshell_close(t_token_type type);
int		token_is_command(t_token_type type);
int		token_is_newline_or_eof(t_token_type type);
int		parse_redirection(t_redir **redir_head, t_token_type token_type,
			t_token *filename_token);
int		parse_simple_command(t_argv **argv_head, t_token *command_token);
int		parse_command_list(t_cmd *cmd, t_token **cur_token);
t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token);
t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token);
t_ast	*gen_command_node(t_ast *parent, t_token **cur_token);
t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token);
void	fgen_tree(t_ast **parent, t_token **cur_token);

// comprehensive tests should check

// parsing new node is done.
// cur_token is cunsumed correctly.
// AST has right structure after parsing.
// no leak.

#include "../../includes/minishell.h"

int	token_is_redirection(t_token_type type)
{
	if (type == TK_REDIRECT_IN || type == TK_REDIRECT_OUT || type == TK_HEREDOC
		|| type == TK_APPEND)
		return (1);
	return (0);
}

int	parse_redirection(t_redir **redir_head, t_token_type token_type,
		t_token *filename_token)
{
	t_redir	*new_redir;
	t_redir	*redir;

	if (!redir_head || !filename_token)
		return (-1);
	redir = *redir_head;
	new_redir = alloc_redir();
	if (!new_redir)
		return (-1);
	memset(new_redir, 0, sizeof(t_redir));
	if (redir == NULL)
		*redir_head = new_redir;
	else
	{
		while (redir->next)
			redir = redir->next;
		redir->next = new_redir;
	}
	if (token_type == TK_REDIRECT_IN)
		new_redir->type = REDIR_IN;
	else if (token_type == TK_REDIRECT_OUT)
		new_redir->type = REDIR_OUT;
	else if (token_type == TK_HEREDOC)
		new_redir->type = REDIR_HEREDOC;
	else
		new_redir->type = REDIR_APPEND;
	new_redir->filename = strdup(filename_token->value);
	if (!new_redir->filename)
		return (-1);
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (filename_token->in_dquote || filename_token->in_squote)
			new_redir->delim_quoted = true;
	}
	return (1);
}

// typedef struct s_argv
// {
// 	char			*word;
// 	bool			to_expand;
// 	struct s_argv	*next;
// }					t_argv;

int	parse_simple_command(t_argv **argv_head, t_token *command_token)
{
	t_argv	*new_argv;

	if (!argv_head || !command_token)
		return (-1);
	new_argv = alloc_argv();
	if (!new_argv)
		return (-1);
	memset(new_argv, 0, sizeof(t_argv));
	new_argv->word = strdup(command_token->value);
	if (!new_argv->word)
	{
		free(new_argv);
		return (-1);
	}
	// TK_DOLLER is single '$' print token.
	if (command_token->in_squote == false && command_token->type != TK_DOLLER)
		new_argv->to_expand = true;
	// Prepend to list (since tokens are in reverse order from lexer)
	new_argv->next = *argv_head;
	*argv_head = new_argv;
	return (1);
}

int	parse_command_list(t_cmd *cmd, t_token **cur_token)
{
	t_token	*tmp;
	int		parse_success;

	if (!cmd || !cur_token || !*cur_token)
		return (-1);
	parse_success = 0;
	tmp = *cur_token;
	// Forward reading: process current and following commands
	while (token_is_command(tmp->type) || token_is_redirection(tmp->type))
	{
		if (token_is_redirection(tmp->type))
		{
			// Redirection: next token is the filename
			if (!tmp->next)
				return (-1);
			parse_success = parse_redirection(&cmd->redir, tmp->type,
					tmp->next);
			tmp = tmp->next; // Skip the filename token
		}
		else
			parse_success = parse_simple_command((t_argv **)&cmd->argv, tmp);
		if (parse_success == 0)
		{
			printf("parse %s failed\n", tmp->value);
			return (-1);
		}
		tmp = tmp->next; // Move forward
		if (!tmp)
			break ;
	}
	// After parsing,
	// tmp points to the first non-command token (operator/EOF/etc)
	*cur_token = tmp;
	return (1);
}

int	token_is_operator(t_token_type type)
{
	if (type == TK_AND_IF || type == TK_OR_IF || type == TK_PIPE)
		return (1);
	return (0);
}

int	token_is_subshell_close(t_token_type type)
{
	if (type == TK_LPAREN)
		return (1);
	return (0);
}

int	token_is_command(t_token_type type)
{
	if (type == TK_WORD || type == TK_DOLLER)
		return (1);
	return (0);
}

int	token_is_newline_or_eof(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_EOF)
		return (1);
	return (0);
}

t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;

	if (!parent || !cur_token || !*cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	if ((*cur_token)->type == TK_AND_IF)
		node->type = NODE_AND;
	else if ((*cur_token)->type == TK_OR_IF)
		node->type = NODE_OR;
	else if ((*cur_token)->type == TK_PIPE)
		node->type = NODE_PIPE;
	// Forward reading: attach previous tree as LEFT child
	if (*parent)
	{
		node->left = *parent;
		node->parent = (*parent)->parent;
		(*parent)->parent = node;
	}
	// Move to next token (right side of operator)
	*cur_token = (*cur_token)->next;
	return (node);
}

t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*subroot;

	if (!cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_SUBSHELL;
	subroot = NULL;
	fgen_tree(&subroot, cur_token);
	node->subtree = subroot;
	return (node);
}

t_ast	*gen_command_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	int		result;

	if (!cur_token)
		return (NULL);
	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_CMD;
	node->cmd = alloc_cmd();
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	memset(node->cmd, 0, sizeof(t_cmd));
	result = parse_command_list(node->cmd, cur_token);
	if (result == -1)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	return (node);
}

// end of command or command line.
t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token)
{
	t_ast	*node;

	(void)parnt;
	(void)cur_token;
	node = NULL;
	return (node);
}

// this is not for real usage.
int	syntax_check(t_token *token)
{
	(void)token;
	return (1);
}

void	fgen_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*op_node;

	if (!parent || !cur_token || !*cur_token
		|| token_is_newline_or_eof((*cur_token)->type)
		|| (*cur_token)->type == TK_HEAD)
		return ;
	// Build current node based on token type
	node = NULL;
	if (token_is_command((*cur_token)->type))
		node = gen_command_node(*parent, cur_token);
	else if (token_is_subshell_close((*cur_token)->type)
		&& syntax_check(*cur_token))
		node = gen_subshell_node(*parent, cur_token);
	else
		node = gen_eof_newline_node(*parent, cur_token);
	if (!node)
		return ;
	// Check if next token is an operator
	if (*cur_token && token_is_operator((*cur_token)->type))
	{
		// Create operator node and make current node its left child
		op_node = swap_with_parent(&node, cur_token);
		if (!op_node)
		{
			*parent = node;
			return ;
		}
		// Now recurse to build the right side
		fgen_tree(&op_node->right, cur_token);
		if (op_node->right)
			op_node->right->parent = op_node;
		*parent = op_node;
	}
	else
	{
		// No operator, just return the node
		*parent = node;
	}
}

t_ast	*parse(t_token *token_head)
{
	t_ast	*root;
	t_token	*cur;

	root = NULL;
	if (!token_head || token_head->type != TK_HEAD)
		return (NULL);
	cur = token_head->next; // Start from first token after HEAD
	if (!cur || cur->type == TK_EOF)
		return (NULL); // Empty input
	fgen_tree(&root, &cur);
	// Verify we consumed everything (should be at EOF or HEAD)
	if (cur && cur->type != TK_EOF && cur->type != TK_HEAD)
	{
		// Parse error: didn't consume all tokens
		// TODO: free_ast(root) when free function is implemented
		return (NULL);
	}
	return (root);
}
