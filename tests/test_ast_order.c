#include "includes/minishell.h"
#include <stdio.h>

t_ast *parse(t_token *token_head);

void print_traversal(t_ast *node, int depth, const char *side) {
    int i;
    t_argv *arg;
    
    if (!node) return;
    
    for (i = 0; i < depth; i++) printf("  ");
    printf("[%s] ", side);
    
    if (node->type == NODE_CMD) {
        printf("CMD: ");
        arg = (t_argv *)node->cmd->argv;
        while (arg) {
            printf("%s ", arg->word);
            arg = arg->next;
        }
        printf("\n");
    } else if (node->type == NODE_PIPE) {
        printf("PIPE\n");
        print_traversal(node->left, depth + 1, "LEFT");
        print_traversal(node->right, depth + 1, "RIGHT");
    } else if (node->type == NODE_AND) {
        printf("AND\n");
        print_traversal(node->left, depth + 1, "LEFT");
        print_traversal(node->right, depth + 1, "RIGHT");
    } else if (node->type == NODE_OR) {
        printf("OR\n");
        print_traversal(node->left, depth + 1, "LEFT");
        print_traversal(node->right, depth + 1, "RIGHT");
    }
}

int main() {
    t_token *head;
    t_ast *root;
    
    printf("=== Test: echo hello | cat ===\n");
    head = lexer("echo hello | cat");
    root = parse(head);
    print_traversal(root, 0, "ROOT");
    
    printf("\n=== Test: echo a && echo b ===\n");
    head = lexer("echo a && echo b");
    root = parse(head);
    print_traversal(root, 0, "ROOT");
    
    printf("\n=== Test: a | b | c ===\n");
    head = lexer("a | b | c");
    root = parse(head);
    print_traversal(root, 0, "ROOT");
    
    return 0;
}
