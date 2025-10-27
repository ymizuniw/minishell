#include "../../../includes/minishell.h"

bool	is_builtin(const char *cmd)
{
	if (!cmd)
		return (1);
	return (strncmp(cmd, "echo", 4) == 0 || strncmp(cmd, "cd", 2) == 0
		|| strncmp(cmd, "pwd", 3) == 0 || strncmp(cmd, "export", 6) == 0
		|| strncmp(cmd, "unset", 5) == 0 || strncmp(cmd, "env", 3) == 0
		|| strncmp(cmd, "exit", 4) == 0);
}
