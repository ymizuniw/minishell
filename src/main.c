#include "../includes/minishell.h"

void	cpy_env(char ***env, char **envp)
{
	size_t	env_size;
	size_t	i;

	if (envp == NULL || *envp == NULL)
		return ;
	env_size = 0;
	while (envp[env_size])
		env_size++;
	*env = malloc(sizeof(char *) * (env_size + 1));
	if (!*env)
		return ;
	i = 0;
	while (i < env_size)
	{
		(*env)[i] = strdup(envp[i]);
		i++;
	}
	(*env)[env_size] = NULL;
}

int	shell_loop(char **envp)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	char		**env;

	env = NULL;
	cpy_env(&env, envp);
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = lexer(line); // format analysis.
		ast = parser(tokens); // abstract structure tree.
		t_result *res = executor(ast, env);  // Commented out since executor not implemented
		free(line);
		if (tokens)
			free_token_list(tokens);
		if (ast)
			free_ast_tree(ast);
		if (res)
			free_result(res);
	}
	if (env)
		free_double_array(env);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;  // Suppress unused parameter warning
	(void)argv;  // Suppress unused parameter warning
	shell_loop(env);
	return (0);
}
