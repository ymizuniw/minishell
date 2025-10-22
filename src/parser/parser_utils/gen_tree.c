#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF || type == TK_LPAREN || type == TK_RPAREN)
		return (1);
	return (0);
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

// generate a tree of command.
// manage corrent token by having the ptr's address.
t_ast	*gen_tree(t_ast *ast, t_token **tail_token, int subshell, int pipeline)
{
	t_ast	*node;
	t_token	*token;
	t_token	*next_token;

	if (!tail_token || !*tail_token)
		return (NULL);
	token = *tail_token;
	next_token = NULL;
	node = alloc_node();
	if (!node)
		return (NULL);
	bzero(node, sizeof(t_ast));
	if (token->type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token->type == TK_OR_IF)
		node->type = NODE_OR;
	else if (token->type == TK_PIPE)
	{
		node->type = NODE_PIPE;
		pipeline = 1;
	}
	else
		node->type = NODE_CMD;
	node->parent = ast;
	if (pipeline == 1)
	{
		node->pipeline = malloc(sizeof(t_pipeline));
		if (node->pipeline)
		{
			bzero(node->pipeline, sizeof(t_pipeline));
			node->pipeline->in_pipeline = true;
		}
	}
	if (is_operator(token->type))
	{
		node = swap_and_set_right_node(node, ast);
		*tail_token = token->next;
		next_token = *tail_token;
		node->left = gen_tree(NULL, &next_token, subshell, pipeline);
		if (node->left)
			node->left->parent = node;
		*tail_token = next_token;
		return (node);
	}
	else if (token->type == TK_LPAREN)
	{
		node->type = NODE_SUBSHELL;
		if (syntax_check(token) != 1)
			return (NULL);
		*tail_token = token->next;
		next_token = *tail_token;
		node->subtree = gen_tree(NULL, &next_token, 1, pipeline);
		if (node->subtree == NULL)
			return (NULL);
		*tail_token = next_token;
		return (node);
	}
	else if (token->type == TK_RPAREN)
	{
		if (subshell == 1)
		{
			*tail_token = token->next;
			return (ast);
		}
		else
		{
			syntax_error(TK_RPAREN);
			return (NULL);
		}
	}
	else if (token->type == TK_WORD || token->type == TK_DOLLER)
	{
		node->cmd = alloc_cmd();
		if (!node->cmd)
			return (NULL);
		bzero(node->cmd, sizeof(t_cmd));
		node->cmd->redir = parse_redirection(node->cmd->redir, tail_token);
		set_argv(node->cmd->argv, tail_token, 0);
		node->cmd->argv[0] = NULL;
	}
	return (node);
}
