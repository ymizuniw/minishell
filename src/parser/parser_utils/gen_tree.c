#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF)
		return (1);
	return (0);
}

t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TK_REDIRECT_IN)
		return (REDIR_IN);
	if (token_type == TK_REDIRECT_OUT)
		return (REDIR_OUT);
	if (token_type == TK_HEREDOC)
		return (REDIR_HEREDOC);
	if (token_type == TK_APPEND)
		return (REDIR_APPEND);
	return (REDIR_OTHER);
}

t_ast	*swap_and_set_right_node(t_ast *new_parent, t_ast *old_parent)
{
	if (!new_parent)
		return (old_parent);
	if (!old_parent)
		return (new_parent);
	new_parent->right = old_parent;
	old_parent->parent = new_parent;
	return (new_parent);
}


// --------------------------------------------------
// classify the type of node based on token type
// --------------------------------------------------
void set_node_type(t_ast *node, t_token_type token_type)
{
	if (token_type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token_type == TK_OR_IF)
		node->type = NODE_OR;
	else if (token_type == TK_PIPE)
		node->type = NODE_PIPE;
	else
		node->type = NODE_CMD;
}

t_ast *sort_by_logical_precedence(t_ast *parent)
{
	t_ast	*node;
	t_ast	*root;

	root = parent;
	while (root && root->parent && (root->parent->type!=NODE_AND && root->parent->type!=NODE_OR))
		root = root->parent;
	node = swap_and_set_right_node(node, root);
	if (node==NULL)
		assert(node==NULL);
	return (node);
}

int init_node(t_ast **node, t_ast *parent, t_token_type token_type)
{
	*node = alloc_node();
	if (!*node)
		return (-1);
	memset(*node, 0, sizeof(t_ast));
	set_node_type(*node, token_type);
	(*node)->parent = parent;
	return (1);
}

t_ast  *sort_and_gen_node(t_ast *parent, t_token **cur_token, t_token *next_token, int subshell)
{
	t_token *token = *cur_token;
	t_ast *node;

	if (init_node(&node, parent, token->type)<0)
		return (NULL);
	if (token->type == TK_AND_IF || token->type == TK_OR_IF)
		node = sort_by_logical_precedence(parent);
	else
		node = swap_and_set_right_node(node, parent);
	if (token && token->next)
	{
		*cur_token = token->next;
		next_token = *cur_token;
	}
	node->left = gen_tree(node, &next_token, subshell);
	if (node->left)
		node->left->parent = node;
	*cur_token = next_token;
	return (node);
}

t_ast  *gen_subshell_tree(t_ast *parent, t_token **cur_token, t_token *next_token, int subshell)
{
	t_token *token = *cur_token;
	t_ast *node;

	if (init_node(&node, parent, token->type)<0)
		return (NULL);
	if (syntax_check(token) != 1)
		return (NULL);
	*cur_token = token->next;
	next_token = *cur_token;
	node->subtree = gen_tree(NULL, &next_token, 1);
	if (!node->subtree)
	{
		free(node);
		return (NULL);
	}
	*cur_token = next_token;
	return (node);
}

t_ast *subshell_close(t_ast *parent, t_token **cur_token, int subshell)
{

	t_token *token = *cur_token;
	t_ast *node;

	if (init_node(&node, parent, token->type)<0)
		return (NULL);
	if (subshell == 1)
	{
		*cur_token = token->next;
		return (node);
	}
	else
	{
		syntax_error(TK_RPAREN);
		return (NULL);
	}
}


//parse_command_list
//

int init_redir(t_redir **redir, t_redir_type redir_type)
{
	*redir = alloc_redir();
	if (*redir==NULL)
		return (-1);
	memset(redir, 0, sizeof(t_redir));
	(*redir)->type = redir_type;
	return (1);
}

int parse_redirection(t_ast *node, t_token **cur_token, t_redir_type redir_type)
{
	t_redir *redir;

	if (!syntax_check(*cur_token))
		return (-1);
	if (init_redir(redir, redir_type)<0)
		return (-1);
	if ((*cur_token)->prev)
		redir->filename = strdup((*cur_token)->prev->value);
	if (!node->cmd->redir)
		node->cmd->redir = redir;
	else
	{
		t_redir *tmp = node->cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	*cur_token = (*cur_token)->prev ? (*cur_token)->prev->prev : NULL;
}

void parse_simple_command(t_ast *node, t_token **cur_token, size_t *i)
{
	set_argv(&node->cmd->argv, cur_token, *i);
	(*i)++;
	*cur_token = (*cur_token)->prev;
}

int init_command_node(t_ast *parent, t_ast **node, t_token_type token_type)
{
	if (init_node(node, parent, token_type)<0)
		return (-1);
	(*node)->cmd = alloc_cmd();
	if (!(*node)->cmd)
		return (-1);
	memset((*node)->cmd, 0, sizeof(t_cmd));
	return (1);
}

int parse_redir_and_command(t_ast *node, t_token *cur, t_token **cur_token, size_t *i)
{
	while (cur && cur!=*cur_token)
	{
		t_redir_type type = get_redir_type(cur->type);
		if (type != REDIR_OTHER)
		{
			if (parse_redirection(node, &cur, type)<0)
				return (-1);
			continue ;
		}
		if (cur->type == TK_WORD || cur->type == TK_DOLLER)
		{
			parse_simple_command(node, cur, i);
			continue ;
		}
		cur = cur->prev;
	}
	return(1);
}

t_ast *parse_command_list(t_ast *parent, t_token **cur_token, int subshell)
{
	size_t		i = 0;
	t_token		*command_start;
	t_token		*cur = *cur_token;
	t_ast 		*node;

	if (init_command_node(parent, &node, cur->type)<0)
		return (NULL);
	command_start = cur;
	while (command_start->next && !is_operator(command_start->next->type))
		command_start = command_start->next;
	cur = command_start;
	if (parse_redir_and_command(node, cur, cur_token, &i)<0)
		return (NULL);
	*cur_token = command_start->next;//meta token pointer.
	return (node);
}


// -----------------------------------------------------------------------------
// generate a tree of command.
// -----------------------------------------------------------------------------
t_ast	*gen_tree(t_ast *parent, t_token **cur_token, int subshell)
{
	t_token	*token;
	t_token	*next_token;

	if ((*cur_token)->type == TK_NEWLINE)
		return (NULL);
	if (!cur_token || !*cur_token)
		return (NULL);

	token = *cur_token;
	next_token = NULL;

	// --------------------------------------------------
	// operator case: &&, ||, |
	// --------------------------------------------------
	//set parent node as right node of the operator. Also, if it is a logical operator token,
	//set the root node of the current operator's right node.
	if (is_operator(token->type))
		return (sort_and_gen_node(parent, cur_token, next_token, subshell));

	// --------------------------------------------------
	// subshells
	// --------------------------------------------------
	//if right parenthesis comes, it is the start of the subshell. do syntax check and generate subshell tree from the node->subtree.
	//set subshell flag to 1.
	
	else if (token->type == TK_RPAREN)
		return (gen_subshell_tree(parent, cur_token, next_token, subshell));
	//if left parenthesis comes, it is the end of the subshell, or invalid token. 
	else if (token->type == TK_LPAREN)
		return (subshell_close(parent, cur_token, subshell));
	// --------------------------------------------------
	// command (WORD / $)
	// --------------------------------------------------
	//if word token or doller token comes, parse redirection and command. to process them in order, proceed token to the end of the command_list,
	else if (token->type == TK_WORD || token->type == TK_DOLLER)
		return (parse_command_list(parent, cur_token, subshell));
	printf("gen_tree encounter not matching error\n");
	return (NULL);
}
