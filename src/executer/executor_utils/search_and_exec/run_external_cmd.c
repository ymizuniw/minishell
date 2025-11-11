#include "../../../../includes/minishell.h"

static void	free_envp(char **envp)
{
	size_t	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		xfree(envp[i]);
		i++;
	}
	xfree(envp);
}

static char	*create_env_string(const char *key, const char *value)
{
	char	*str;
	size_t	key_len;
	size_t	value_len;
	size_t	len;

	key_len = 0;
	value_len = 0;
	len = 0;
	if (key)
		key_len = strlen(key);
	if (value)
		value_len = strlen(value);
	len = key_len + value_len + 2;
	str = xmalloc(len);
	if (!str)
		return (NULL);
	snprintf(str, len, "%s=%s", key, value);
	return (str);
}

static size_t	count_exported_vars(t_env *env_list)
{
	t_env	*current;
	size_t	count;

	current = env_list;
	count = 0;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	return (count);
}

static int	add_env_to_array(char **envp, t_env *env_list)
{
	t_env	*current;
	size_t	i;

	current = env_list;
	i = 0;
	while (current)
	{
		if (current->exported)
		{
			envp[i] = create_env_string(current->key, current->value);
			if (!envp[i])
				return (free_envp(envp), 0);
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (1);
}

char	**generate_envp(t_env *env_list)
{
	char	**envp;
	size_t	count;

	count = count_exported_vars(env_list);
	envp = xmalloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (!add_env_to_array(envp, env_list))
		return (NULL);
	return (envp);
}

static void	exec_in_pipe_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	execve(cmd_path, cmd_args, envp);
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	ft_exit(cmd_args, 127, shell);
}

static void	exec_in_fork_child(char *cmd_path, char **cmd_args, char **envp,
		t_shell *shell)
{
	set_sig_dfl();
	execve(cmd_path, cmd_args, envp);
	printf("execve returned\n");
	perror("execve");
	xfree(cmd_path);
	free_envp(envp);
	ft_exit(cmd_args, 127, shell);
}

void	search_in_path_and_exec(t_shell *shell, char **cmd_args)
{
	pid_t	pid;
	char	*cmd_path;
	char	**envp;

	if (!cmd_args || !cmd_args[0])
		return ;
	cmd_path = find_command_path(shell, cmd_args[0]);
	if (!cmd_path || !cmd_path[0])
		return ;
	envp = generate_envp(shell->env_list);
	if (!envp)
		return (xfree(cmd_path), (void)0);
	if (shell->in_pipe_child)
	{
		exec_in_pipe_child(cmd_path, cmd_args, envp, shell);
		return ;
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), xfree(cmd_path), free_envp(envp),
			(void)(shell->last_exit_status = 1));
	if (pid == 0)
		exec_in_fork_child(cmd_path, cmd_args, envp, shell);
	xfree(cmd_path);
	free_envp(envp);
	handle_child(&shell->last_exit_status, pid);
}
