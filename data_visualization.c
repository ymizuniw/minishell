#include <minishell.h>

void print_token(t_token    *token)
{
    printf("size: %zu\n", token->size);
    printf("count_newline: %zu\n", token->count_newline);
    printf("type: %d\n", token->type);
    printf("in_squote: %d\n", token->in_squote);
    printf("in_dquote: %d\n", token->in_dquote);
    printf("valud: %s\n", token->value);
}

void print_token_list(t_token *token)
{
    if (token->next)
        print_token(token->next);
    print_token(token);
}

void print_word(t_word *word)
{
	printf("word: %s\n", word->word);
	printf("type: %d\n", word->type);
	printf("to_expand_doller: %d\n", word->to_expand_doller);
	printf("quoted: %d\n", word->quoted);
}

void print_word_list(t_word *word)
{
	if (word->next)
		print_word_list(word->next);
	print_word(word);
}

void print_redir(t_redir *redir)
{
	printf("type: %d\n", redir->type);
	printf("filename: %s\n", redir->filename);
	printf("delim_quoted: %d\n", redir->delim_quoted);
	printf("tmp_fd: %d\n", redir->tmp_fd);
}

void print_redir_list(t_redir *redir)
{
	if (redir->next)
		print_redir_list(redir->next);
	print_redir(redir);
}

void print_tokens(t_token **tokens)
{
    if (!tokens)
    {
        printf("(tokens: NULL)\n");
        return;
    }

    printf("tokens:\n");
    size_t i = 0;
    while (tokens[i])
    {
        printf("  tokens[%zu]:\n", i);
        print_token(tokens[i]);
        i++;
    }
}
void print_argv(char **argv)
{
    if (!argv)
    {
        printf("(argv: NULL)\n");
        return;
    }

    printf("argv:\n");
    size_t i = 0;
    while (argv[i])
    {
        printf("  argv[%zu]: %s\n", i, argv[i]);
        i++;
    }
}

void print_ast(t_ast *ast)
{
	printf("AST Node\n");
	printf("type: %d\n", ast->type);
	printf("parent: %p\n", (void *)ast->parent);
	printf("left: %p\n", (void *)ast->left);
	printf("right: %p\n", (void *)ast->right);
	printf("subtree: %p\n", (void *)ast->subtree);
	printf("cmd: %p\n", (void *)ast->cmd);
	printf("\n");
}

void print_ast_recursive(t_ast *node)
{
	if (!node)
		return;
	print_ast(node);
	print_ast_recursive(node->left);
	print_ast_recursive(node->right);
	print_ast_recursive(node->subtree);
}
