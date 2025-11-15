#include "../../includes/minishell.h"

void	free_env_list(t_env *env_list);
void	free_ast_tree(t_ast *node);
void	free_token_list(t_token *p);

// Cleanup all shell data before execve()
// This frees everything except what execve() needs (cmd_args and envp)
void	cleanup_before_execve(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->stdin_backup >= 0)
		close(shell->stdin_backup);
	if (shell->stdout_backup >= 0)
		close(shell->stdout_backup);
	if (shell->hist)
		free_hist_box(shell->hist->hist_box);
	free_env_list(shell->env_list);
	shell->env_list = NULL;
	free_ast_tree(shell->root);
	shell->root = NULL;
	free_token_list(shell->token_list);
	shell->token_list = NULL;
	xfree(shell->line_ptr);
	shell->line_ptr = NULL;
	xfree(shell->pwd);
	shell->pwd = NULL;
}
